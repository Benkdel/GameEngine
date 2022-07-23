#version 460 core

uniform sampler2D diffuse0;
uniform sampler2D specular0;

struct DirLight {
	vec3 direction;

	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
};
uniform DirLight dirLight;

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

uniform vec3 viewPos;

vec4 calcDirLight(vec3 norm, vec3 viewDir, vec4 texDiff, vec4 texSpec);

void main() {
	// properties
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);

	vec4 texDiff;
	vec4 texSpec;

	texDiff = texture(diffuse0, TexCoord);
	texSpec = texture(specular0, TexCoord);
	
	vec4 result;

	// directional
	result = calcDirLight(norm, viewDir, texDiff, texSpec);

	FragColor = result;
}

vec4 calcDirLight(vec3 norm, vec3 viewDir, vec4 texDiff, vec4 texSpec) {
	// ambient
	vec4 ambient = dirLight.ambient * texDiff;

	// diffuse
	vec3 lightDir = normalize(-dirLight.direction);
	float diff = max(dot(norm, lightDir), 0.0);
	vec4 diffuse = dirLight.diffuse * (diff * texDiff);

	// specular
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 0.5 * 128);
	vec4 specular = dirLight.specular * (spec * texSpec);

	return vec4(ambient + diffuse + specular);
}
