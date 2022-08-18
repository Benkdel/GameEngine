#version 460 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

uniform sampler2D u_Texture;

out vec4 FragColor;

void main()
{
	vec4 texColor = texture(u_Texture, TexCoord);
	if (texColor.a < 0.1)
		discard;
	FragColor = texColor;
}
