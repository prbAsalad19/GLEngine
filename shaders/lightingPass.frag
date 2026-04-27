#version 460 core

out vec4 FragColor;
in vec2 TexCoords;

// --- GBuffer Samplers (Binding espliciti 460 core) ---
layout(binding = 0) uniform sampler2D u_gNormal;   // RG16F (Octahedron encoded)
layout(binding = 1) uniform sampler2D u_gAlbedo;   // RGBA8
layout(binding = 2) uniform sampler2D u_gMaterial; // RGBA8 (R=Rough, G=Metal, B=AO)
layout(binding = 3) uniform sampler2D u_gDepth;    // Depth24

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

// --- SSBO Strutture Luci ---
struct GPULight {
    vec4 position; // w = radius
    vec4 color;    // w = intensity
};

layout(std430, binding = 3) buffer LightGrid { uvec2 clusters[]; };
layout(std430, binding = 4) buffer LightIndexList { uint globalIndexList[]; };
layout(std430, binding = 5) buffer LightData {
    uint lightCount;
    uint _pad[3];
    GPULight lights[];
};

// --- DECODIFICA OCTAHEDRON ---
// Questa funzione inverte esattamente quello che fai nel geometry shader
vec3 octDecode(vec2 f) {
    vec3 v = vec3(f.xy, 1.0 - abs(f.x) - abs(f.y));
    if (v.z < 0.0) {
        v.xy = (1.0 - abs(v.yx)) * vec2(v.x >= 0.0 ? 1.0 : -1.0, v.y >= 0.0 ? 1.0 : -1.0);
    }
    return normalize(v);
}

vec3 getPositionFromDepth(float depth) {
    vec4 ndc = vec4(TexCoords * 2.0 - 1.0, depth * 2.0 - 1.0, 1.0);
    vec4 worldPos = inverseView * inverseProjection * ndc;
    return worldPos.xyz / worldPos.w;
}

void main() {
    float depth = texture(u_gDepth, TexCoords).r;
    if (depth >= 1.0) discard;

    // 1. Decodifica dei dati
    vec2 encodedNormal = texture(u_gNormal, TexCoords).rg;
    vec3 Normal = octDecode(encodedNormal); // <--- DECODIAMO QUI
    
    vec3 worldPos = getPositionFromDepth(depth);
    vec4 albedoTex = texture(u_gAlbedo, TexCoords);
    vec3 Albedo = albedoTex.rgb;
    
    vec4 mat = texture(u_gMaterial, TexCoords);
    float Roughness = mat.r;
    float Metallic  = mat.g;
    float AO        = mat.b;

    // 2. Clustered Logic
    vec4 viewPos = view * vec4(worldPos, 1.0);
    float zView = -viewPos.z; 
    uint zSlice = uint(max(0.0, log(zView / zNear) * float(gridRes.z) / log(zFar / zNear)));
    
    uvec3 clusterCoords = uvec3(
        uvec2(gl_FragCoord.xy / screenRes * vec2(gridRes.xy)),
        zSlice
    );
    uint clusterIndex = clusterCoords.x + 
                        clusterCoords.y * gridRes.x + 
                        clusterCoords.z * gridRes.x * gridRes.y;

    // 3. Lighting (PBR Semplificato)
    vec3 V = normalize(cameraPosition - worldPos);
    vec3 lighting = Albedo * 0.02 * AO; // Ambientale pesata dall'AO
    
    uvec2 lightIndices = clusters[clusterIndex];
    for (uint i = 0; i < lightIndices.y; i++) {
        uint lightIdx = globalIndexList[lightIndices.x + i];
        GPULight light = lights[lightIdx];

        vec3 L = light.position.xyz - worldPos;
        float dist = length(L);
        float radius = light.position.w;
        
        if (dist < radius) {
            L /= dist; // normalize
            vec3 H = normalize(V + L);
            float attenuation = pow(clamp(1.0 - (dist / radius), 0.0, 1.0), 2.0);
            vec3 radiance = light.color.rgb * light.color.a * attenuation;

            float nDotL = max(dot(Normal, L), 0.0);
            vec3 diffuse = Albedo * nDotL;
            
            // Speculare veloce
            float nDotH = max(dot(Normal, H), 0.0);
            float spec = pow(nDotH, mix(10.0, 256.0, 1.0 - Roughness)) * Metallic;

            lighting += (diffuse + spec) * radiance;
        }
    }

    // 4. Final Color (Gamma 2.2)
    FragColor = vec4(pow(lighting / (lighting + 1.0), vec3(1.0/2.2)), 1.0);
}