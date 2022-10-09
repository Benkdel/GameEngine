#version 460 core

layout (location = 0) out vec4 FragColor;

in vec3 v_Normals;
in vec2 v_TexCoord;

void main()
{
	FragColor = vec4(0.3f, 0.5f, 0.8f, 1.0f);
}
