#version 460 core

in vec3 TexCoords;

uniform samplerCube u_Skybox;

out vec4 FragColor;

void main()
{
	FragColor = texture(u_Skybox, TexCoords);
}
