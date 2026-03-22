#version 460 core

in vec2 fragUV;
in vec4 fragColor;

out vec4 screenColor;

void main()
{
    screenColor = fragColor;
}