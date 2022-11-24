#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 texCoord;
layout (location = 3) in mat4 aInstanceMatrix;

out vec3 WorldPos;
out vec3 Normal;
out vec2 TexCoord;

uniform mat4 u_ViewProjection;
uniform mat4 u_Perspective;
uniform mat4 u_Transform;

void main()
{
	WorldPos = vec3(u_Transform * vec4(aPos, 1.0));
	Normal = mat3(transpose(inverse(u_Transform))) * aNormal;

	TexCoord = texCoord;

	gl_Position = u_Perspective * u_ViewProjection * aInstanceMatrix * vec4(WorldPos, 1.0);
};
