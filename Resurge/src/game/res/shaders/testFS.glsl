#version 460 core

layout (location = 0) out vec4 FragColor;

uniform float u_Green;

void main()
{
	FragColor = vec4(0.5f, u_Green, 0.2f, 1.0f);
}
