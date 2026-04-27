#version 460 core

layout(location = 0) out vec2 outNormal;
layout(location = 1) out vec4 outAlbedo;
layout(location = 2) out vec4 outMaterial;

in vec3 fragPosition;
in vec3 fragNormal;
in vec2 fragUV;

layout(binding = 0) uniform sampler2D albedoMap;
layout(binding = 1) uniform sampler2D roughnessMap;
layout(binding = 2) uniform sampler2D metallicMap;
layout(binding = 3) uniform sampler2D aoMap;

vec2 octWrap(vec2 v) 
{
    return (1.0 - abs(v.yx)) * sign(v);
}

void main() 
{
    outAlbedo = texture(albedoMap, fragUV).rgba;
    float roughness = texture(roughnessMap, fragUV).r;
    float metallic = texture(metallicMap, fragUV).r;
    float ao = texture(aoMap, fragUV).r;
    outMaterial = vec4(roughness, metallic, ao, 0.0);

    vec2 normalProjection = fragNormal.xy / 
                        (abs(fragNormal.x) + abs(fragNormal.y) +abs(fragNormal.z));
    outNormal = (fragNormal.z >= 0.0) ? normalProjection : octWrap(normalProjection);
}