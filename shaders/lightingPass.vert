#version 460 core

out vec2 TexCoords;

void main() {
    // Genera un triangolo che racchiude lo schermo
    // ID 0: (-1, -1) -> UV (0, 0)
    // ID 1: ( 3, -1) -> UV (2, 0)
    // ID 2: (-1,  3) -> UV (0, 2)
    float x = -1.0 + float((gl_VertexID & 1) << 2);
    float y = -1.0 + float((gl_VertexID & 2) << 1);
    
    TexCoords = vec2((x + 1.0) * 0.5, (y + 1.0) * 0.5);
    gl_Position = vec4(x, y, 0.0, 1.0);
}