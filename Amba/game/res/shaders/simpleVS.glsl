#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 texCoord;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoord;

uniform mat4 u_ViewProjection;
uniform mat4 u_Perspective;
uniform mat4 u_Transform;

void main()
{
	FragPos = vec3(u_Transform * vec4(aPos, 1.0));
	Normal = mat3(transpose(inverse(u_Transform))) * aNormal;

	gl_Position = u_Perspective * u_ViewProjection * vec4(FragPos, 1.0);
	TexCoord = texCoord;
};