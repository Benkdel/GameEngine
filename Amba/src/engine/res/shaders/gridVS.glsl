#version 460 core

layout (location = 0) in vec3 aPos;

out vec3 FragPos;

uniform mat4 u_ViewProjection;
uniform mat4 u_Perspective;
uniform mat4 u_Transform;

void main()
{
	FragPos = vec3(u_Transform * vec4(aPos, 1.0));
	
	gl_Position = u_Perspective * u_ViewProjection * vec4(FragPos, 1.0);
};
