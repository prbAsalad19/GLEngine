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

void main()
{
    float depth = texture(u_gDepth, TexCoords).r;
    if (depth >= 1.0) discard;

    // --- GBUFFER DECODE ---
    vec3 worldPos = getWorldPosition(depth);

    vec2 encodedNormal = texture(u_gNormal, TexCoords).rg;
    vec3 Normal = normalize(mat3(inverseView) * octDecode(encodedNormal));

    vec3 Albedo = texture(u_gAlbedo, TexCoords).rgb;

    vec4 mat = texture(u_gMaterial, TexCoords);
    float Roughness = mat.r;
    float Metallic  = mat.g;
    float AO        = mat.b;

    // --- VIEW SPACE ---
    vec3 viewPos = (view * vec4(worldPos, 1.0)).xyz;
    float zView = -viewPos.z;

    // evita NaN nel log
    zView = max(zView, zNear + 0.0001);

    // --- Z SLICE ---
    float zSliceF = log(zView / zNear) * float(gridRes.z) / log(zFar / zNear);
    uint zSlice = uint(clamp(zSliceF, 0.0, float(gridRes.z - 1)));

    // --- XY CLUSTER ---
    vec2 clusterXYf = gl_FragCoord.xy / screenRes * vec2(gridRes.xy);
    uvec2 clusterXY = uvec2(clamp(clusterXYf, vec2(0.0), vec2(gridRes.xy) - 1.0));

    uvec3 clusterCoords = uvec3(clusterXY, zSlice);

    uint clusterIndex =
        clusterCoords.x +
        clusterCoords.y * gridRes.x +
        clusterCoords.z * gridRes.x * gridRes.y;

    // --- LIGHTING ---
    vec3 V = normalize(cameraPosition - worldPos);

    vec3 lighting = Albedo * 0.02; // ambient

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