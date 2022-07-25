#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 texCoord;

out vec3 v_Normals;
out vec2 v_TexCoord;

uniform mat4 u_ViewProjection;
uniform mat4 u_Perspective;
uniform mat4 u_Transform;

void main()
{
	gl_Position = u_Perspective * u_ViewProjection * u_Transform * vec4(aPos, 1.0);
	v_Normals = aNormal;
	v_TexCoord = texCoord;
};
