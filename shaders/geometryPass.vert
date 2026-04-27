#version 460 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;

layout (std140, binding = 0) uniform CameraData {
    mat4 view;                // Offset 0
    mat4 projection;          // Offset 64
    mat4 viewProjection;      // Offset 128
    mat4 inverseView;         // Offset 192
    mat4 inverseProjection;   // Offset 256

    // cameraPosition[3] + zNear occupano esattamente un vec4 (16 byte)
    vec3 cameraPosition;      // Offset 320
    float zNear;              // Offset 332

    // zFar + _pad0[3] occupano un altro vec4
    float zFar;               // Offset 336
    // Il padding di 12 byte qui è implicito perché il prossimo membro (uvec4) 
    // deve iniziare a un offset multiplo di 16 (352).
    
    uvec4 gridRes;            // Offset 352 (X, Y, Z, W/Padding)
    
    vec2 screenRes;           // Offset 368
    float _padding;           // Offset 376
    // L'ultimo float di padding (offset 380) è gestito implicitamente dallo standard std140
    // per chiudere il blocco a 384 byte.
};

layout (std140, binding = 1) uniform TransformData
{
    mat4 models[1024];
};

out vec3 fragPosition;
out vec3 fragNormal;
out vec2 fragUV;

void main()
{
    mat4 model = models[gl_InstanceID];
    gl_Position = viewProjection * model * vec4(position, 1.0);
    fragPosition = (view * model * vec4(position, 1.0)).xyz;
    fragNormal = normalize(mat3(view * model) * normal);
    fragUV = uv;
}