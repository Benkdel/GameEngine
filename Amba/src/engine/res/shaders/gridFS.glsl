#version 460 core

in vec3 FragPos;

uniform vec4 u_Color;

out vec4 FragColor;

void main()
{
	FragColor = u_Color;
}
