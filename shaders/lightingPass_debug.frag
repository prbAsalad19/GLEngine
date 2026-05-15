#version 460 core

out vec4 FragColor;
in vec2 TexCoords;

// --- GBuffer Samplers ---
layout(binding = 0) uniform sampler2D u_gNormal;
layout(binding = 1) uniform sampler2D u_gAlbedo;
layout(binding = 2) uniform sampler2D u_gMaterial;
layout(binding = 3) uniform sampler2D u_gDepth;


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

// --- OCT DECODE ---
vec3 octDecode(vec2 f)
{
    vec3 v = vec3(f.xy, 1.0 - abs(f.x) - abs(f.y));
    if (v.z < 0.0)
        v.xy = (1.0 - abs(v.yx)) * sign(v.xy);
    return normalize(v);
}

// --- CORRECT DEPTH RECONSTRUCTION ---
vec3 getWorldPosition(float depth)
{
    vec4 ndc = vec4(TexCoords * 2.0 - 1.0,
                    depth * 2.0 - 1.0,
                    1.0);

    vec4 viewPos = inverseProjection * ndc;
    viewPos /= viewPos.w;

    vec4 worldPos = inverseView * viewPos;
    return worldPos.xyz;
}

// DEBUG MODE: 
// 0 = normal rendering (full lighting)
// 1 = albedo only 
// 2 = normals
// 3 = depth
// 4 = world position 
// 5 = cluster light count heatmap
// 6 = lightCount from SSBO (should be > 0)
// 7 = direct light (bypass cluster, use all lights)
uniform int u_debugMode;

void main()
{
    float depth = texture(u_gDepth, TexCoords).r;
    if (depth >= 1.0) discard;

    // --- GBUFFER DECODE ---
    vec3 worldPos = getWorldPosition(depth);

    vec2 encodedNormal = texture(u_gNormal, TexCoords).rg;
    vec3 Normal = normalize(mat3(inverseView) * octDecode(encodedNormal));

    vec3 Albedo = texture(u_gAlbedo, TexCoords).rgb;

    vec4 matData = texture(u_gMaterial, TexCoords);
    float Roughness = matData.r;
    float Metallic  = matData.g;
    float AO        = matData.b;

    if (u_debugMode == 1)
    {
        // Show albedo
        FragColor = vec4(Albedo, 1.0);
        return;
    }
    else if (u_debugMode == 2)
    {
        // Show normals
        FragColor = vec4(Normal * 0.5 + 0.5, 1.0);
        return;
    }
    else if (u_debugMode == 3)
    {
        // Show depth
        float linearDepth = (2.0 * zNear) / (zFar + zNear - depth * (zFar - zNear));
        FragColor = vec4(vec3(linearDepth), 1.0);
        return;
    }
    else if (u_debugMode == 4)
    {
        // Show world position (scaled)
        FragColor = vec4(fract(worldPos * 0.1), 1.0);
        return;
    }
    else if (u_debugMode == 5)
    {
        // Show cluster light count heatmap
        vec3 viewPosDbg = (view * vec4(worldPos, 1.0)).xyz;
        float zViewDbg = max(-viewPosDbg.z, zNear + 0.0001);
        float zSliceFDbg = log(zViewDbg / zNear) * float(gridRes.z) / log(zFar / zNear);
        uint zSliceDbg = uint(clamp(zSliceFDbg, 0.0, float(gridRes.z - 1)));
        vec2 clusterXYfDbg = gl_FragCoord.xy / screenRes * vec2(gridRes.xy);
        uvec2 clusterXYDbg = uvec2(clamp(clusterXYfDbg, vec2(0.0), vec2(gridRes.xy) - 1.0));
        uint clusterIdxDbg = clusterXYDbg.x + clusterXYDbg.y * gridRes.x + zSliceDbg * gridRes.x * gridRes.y;
        uvec2 lightIndicesDbg = clusters[clusterIdxDbg];
        float heatmap = float(lightIndicesDbg.y) / 5.0; // scale: 5 lights = fully red
        FragColor = vec4(heatmap, 1.0 - heatmap, 0.0, 1.0);
        return;
    }
    else if (u_debugMode == 6)
    {
        // Show lightCount — green if > 0, red if 0
        if (lightCount > 0u)
            FragColor = vec4(0.0, 1.0, 0.0, 1.0); // GREEN = lights present in SSBO
        else
            FragColor = vec4(1.0, 0.0, 0.0, 1.0); // RED = no lights uploaded
        return;
    }
    else if (u_debugMode == 7)
    {
        // BYPASS CLUSTER — directly iterate all lights
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
                vec3 diffuse = Albedo * nDotL;

                float nDotH = max(dot(Normal, H), 0.0);
                float spec = pow(nDotH, mix(10.0, 256.0, 1.0 - Roughness)) * Metallic;

                lighting += (diffuse + spec) * radiance;
            }
        }

        FragColor = vec4(
            pow(lighting / (lighting + 1.0), vec3(1.0 / 2.2)),
            1.0
        );
        return;
    }
    else
    {
        // --- Normal clustered rendering ---
        vec3 viewPos = (view * vec4(worldPos, 1.0)).xyz;
        float zView = -viewPos.z;
        zView = max(zView, zNear + 0.0001);

        float zSliceF = log(zView / zNear) * float(gridRes.z) / log(zFar / zNear);
        uint zSlice = uint(clamp(zSliceF, 0.0, float(gridRes.z - 1)));

        vec2 clusterXYf = gl_FragCoord.xy / screenRes * vec2(gridRes.xy);
        uvec2 clusterXY = uvec2(clamp(clusterXYf, vec2(0.0), vec2(gridRes.xy) - 1.0));

        uvec3 clusterCoords = uvec3(clusterXY, zSlice);

        uint clusterIndex =
            clusterCoords.x +
            clusterCoords.y * gridRes.x +
            clusterCoords.z * gridRes.x * gridRes.y;

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

                float spotAttenuation = 1.0;
                float cosAngle = dot(-L, normalize(light.direction));
                spotAttenuation = smoothstep(light.outerAngleCos, light.innerAngleCos, cosAngle);
                                
                vec3 H = normalize(V + L);

                float attenuation = pow(clamp(1.0 - (dist / light.radius), 0.0, 1.0), 2.0);
                vec3 radiance = light.color * light.intensity * attenuation * spotAttenuation;

                float nDotL = max(dot(Normal, L), 0.0);
                vec3 diffuse = Albedo * nDotL;

                float nDotH = max(dot(Normal, H), 0.0);
                float spec = pow(nDotH, mix(10.0, 256.0, 1.0 - Roughness)) * Metallic;

                lighting += (diffuse + spec) * radiance;
            }
        }

        FragColor = vec4(
            pow(lighting / (lighting + 1.0), vec3(1.0 / 2.2)),
            1.0
        );
    }
}
