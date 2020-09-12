#version 330 core

out vec4 FragColor;

in float Alpha;

uniform vec3 outlineColor;


void main()
{
    FragColor = vec4(outlineColor, Alpha);
}
