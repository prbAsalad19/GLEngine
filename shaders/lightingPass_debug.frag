#version 460 core

out vec4 FragColor;
in vec2 TexCoords;

// --- GBuffer Samplers ---
layout(binding = 0) uniform sampler2D u_gNormal;
layout(binding = 1) uniform sampler2D u_gAlbedo;
layout(binding = 2) uniform sampler2D u_gMaterial;
layout(binding = 3) uniform sampler2D u_gDepth;
layout(binding = 6) uniform sampler2DShadow u_shadowAtlas;

// --- UBO Camera Data ---
layout (std140, binding = 0) uniform CameraData {
    mat4 view;
    mat4 projection;
    mat4 viewProjection;
    mat4 inverseView;
    mat4 inverseProjection;
    vec3 cameraPosition;
    float zNear;
    float zFar;
    uvec4 gridRes;
    vec2 screenRes;
};

// --- Lights ---
struct GPULight
{
    vec3  position;
    float radius;
    vec3  color;
    float intensity;
    vec3  direction;
    float outerAngleCos;
    uint  type;
    float innerAngleCos;
    float _pad0;
    float _pad1;
};

layout(std430, binding = 3) buffer LightGrid { uvec2 clusters[]; };
layout(std430, binding = 4) buffer LightIndexList { uint globalIndexList[]; };
layout(std430, binding = 5) buffer LightData {
    uint lightCount;
    uint _pad[3];
    GPULight lights[];
};

// --- Shadow Casters ---
struct GPUShadowCaster
{
    mat4  lightSpaceMatrix;
    vec4  atlasRect;
    uint  lightIndex;
    float bias;
    float _pad0;
    float _pad1;
};

layout(std430, binding = 7) readonly buffer ShadowCasterBuffer
{
    uint casterCount;
    GPUShadowCaster casters[];
};

// --- OCT DECODE ---
vec3 octDecode(vec2 f)
{
    vec3 v = vec3(f.xy, 1.0 - abs(f.x) - abs(f.y));
    if (v.z < 0.0)
        v.xy = (1.0 - abs(v.yx)) * sign(v.xy);
    return normalize(v);
}

// --- DEPTH RECONSTRUCTION ---
vec3 getWorldPosition(float depth)
{
    vec4 ndc = vec4(TexCoords * 2.0 - 1.0, depth * 2.0 - 1.0, 1.0);
    vec4 viewPos = inverseProjection * ndc;
    viewPos /= viewPos.w;
    vec4 worldPos = inverseView * viewPos;
    return worldPos.xyz;
}

// --- PCF SHADOW SAMPLE ---
float sampleShadow(uint casterIdx, vec3 worldPos)
{
    GPUShadowCaster caster = casters[casterIdx];
    
    vec4 lightSpacePos = caster.lightSpaceMatrix * vec4(worldPos, 1.0);
    vec3 projCoords = lightSpacePos.xyz / lightSpacePos.w;
    projCoords = projCoords * 0.5 + 0.5;

    if (projCoords.x < 0.0 || projCoords.x > 1.0 ||
        projCoords.y < 0.0 || projCoords.y > 1.0 ||
        projCoords.z > 1.0) return 1.0;

    vec2 atlasUV = caster.atlasRect.xy + projCoords.xy * caster.atlasRect.zw;

    float shadow = 0.0;
    vec2 texelSize = caster.atlasRect.zw / vec2(4096.0);
    
    for (int x = -1; x <= 1; x++)
        for (int y = -1; y <= 1; y++)
            shadow += texture(u_shadowAtlas, 
                vec3(atlasUV + vec2(x, y) * texelSize,
                     projCoords.z - caster.bias));

    return shadow / 9.0;
}

uniform int u_debugMode;

void main()
{
    float depth = texture(u_gDepth, TexCoords).r;
    if (depth >= 1.0) discard;

    vec3 worldPos = getWorldPosition(depth);
    vec2 encodedNormal = texture(u_gNormal, TexCoords).rg;
    vec3 Normal = normalize(mat3(inverseView) * octDecode(encodedNormal));
    vec3 Albedo = texture(u_gAlbedo, TexCoords).rgb;
    vec4 matData = texture(u_gMaterial, TexCoords);
    float Roughness = matData.r;
    float Metallic  = matData.g;
    float AO        = matData.b;

    if (u_debugMode == 1) { FragColor = vec4(Albedo, 1.0); return; }
    else if (u_debugMode == 2) { FragColor = vec4(Normal * 0.5 + 0.5, 1.0); return; }
    else if (u_debugMode == 3)
    {
        float linearDepth = (2.0 * zNear) / (zFar + zNear - depth * (zFar - zNear));
        FragColor = vec4(vec3(linearDepth), 1.0); return;
    }
    else if (u_debugMode == 4) { FragColor = vec4(fract(worldPos * 0.1), 1.0); return; }
    else if (u_debugMode == 5)
    {
        vec3 viewPosDbg = (view * vec4(worldPos, 1.0)).xyz;
        float zViewDbg = max(-viewPosDbg.z, zNear + 0.0001);
        float zSliceFDbg = log(zViewDbg / zNear) * float(gridRes.z) / log(zFar / zNear);
        uint zSliceDbg = uint(clamp(zSliceFDbg, 0.0, float(gridRes.z - 1)));
        vec2 clusterXYfDbg = gl_FragCoord.xy / screenRes * vec2(gridRes.xy);
        uvec2 clusterXYDbg = uvec2(clamp(clusterXYfDbg, vec2(0.0), vec2(gridRes.xy) - 1.0));
        uint clusterIdxDbg = clusterXYDbg.x + clusterXYDbg.y * gridRes.x + zSliceDbg * gridRes.x * gridRes.y;
        uvec2 lightIndicesDbg = clusters[clusterIdxDbg];
        float heatmap = float(lightIndicesDbg.y) / 5.0;
        FragColor = vec4(heatmap, 1.0 - heatmap, 0.0, 1.0); return;
    }
    else if (u_debugMode == 6)
    {
        FragColor = (lightCount > 0u) ? vec4(0.0, 1.0, 0.0, 1.0) : vec4(1.0, 0.0, 0.0, 1.0);
        return;
    }
    else if (u_debugMode == 7)
    {
        vec3 V = normalize(cameraPosition - worldPos);
        vec3 lighting = Albedo * 0.02;
        for (uint i = 0; i < lightCount; i++)
        {
            GPULight light = lights[i];
            vec3 L = light.position - worldPos;
            float dist = length(L);
            if (dist < light.radius)
            {
                L /= dist;
                vec3 H = normalize(V + L);
                float attenuation = pow(clamp(1.0 - (dist / light.radius), 0.0, 1.0), 2.0);
                vec3 radiance = light.color * light.intensity * attenuation;
                float nDotL = max(dot(Normal, L), 0.0);
                float nDotH = max(dot(Normal, H), 0.0);
                float spec = pow(nDotH, mix(10.0, 256.0, 1.0 - Roughness)) * Metallic;
                lighting += (Albedo * nDotL + spec) * radiance;
            }
        }
        FragColor = vec4(pow(lighting / (lighting + 1.0), vec3(1.0 / 2.2)), 1.0);
        return;
    }
    else
    {
        vec3 viewPos = (view * vec4(worldPos, 1.0)).xyz;
        float zView = max(-viewPos.z, zNear + 0.0001);
        float zSliceF = log(zView / zNear) * float(gridRes.z) / log(zFar / zNear);
        uint zSlice = uint(clamp(zSliceF, 0.0, float(gridRes.z - 1)));
        vec2 clusterXYf = gl_FragCoord.xy / screenRes * vec2(gridRes.xy);
        uvec2 clusterXY = uvec2(clamp(clusterXYf, vec2(0.0), vec2(gridRes.xy) - 1.0));
        uint clusterIndex = clusterXY.x + clusterXY.y * gridRes.x + zSlice * gridRes.x * gridRes.y;

        vec3 V = normalize(cameraPosition - worldPos);
        vec3 lighting = Albedo * 0.02;
        uvec2 lightIndices = clusters[clusterIndex];

        for (uint i = 0; i < lightIndices.y; i++)
        {
            uint lightIdx = globalIndexList[lightIndices.x + i];
            GPULight light = lights[lightIdx];

            vec3 L = light.position - worldPos;
            float dist = length(L);

            if (dist < light.radius)
            {
                L /= dist;

                float cosAngle = dot(-L, normalize(light.direction));
                float spotAttenuation = smoothstep(light.outerAngleCos, light.innerAngleCos, cosAngle);
                float attenuation = pow(clamp(1.0 - (dist / light.radius), 0.0, 1.0), 2.0);

                float shadow = 1.0;
                for (uint c = 0; c < casterCount; c++)
                {
                    if (casters[c].lightIndex == lightIdx)
                    {
                        shadow = sampleShadow(c, worldPos);
                        break;
                    }
                }

                vec3 H = normalize(V + L);
                vec3 radiance = light.color * light.intensity * attenuation * spotAttenuation * shadow;

                float nDotL = max(dot(Normal, L), 0.0);
                float nDotH = max(dot(Normal, H), 0.0);
                float spec = pow(nDotH, mix(10.0, 256.0, 1.0 - Roughness)) * Metallic;

                lighting += (Albedo * nDotL + spec) * radiance;
            }
        }

        FragColor = vec4(pow(lighting / (lighting + 1.0), vec3(1.0 / 2.2)), 1.0);
    }
}