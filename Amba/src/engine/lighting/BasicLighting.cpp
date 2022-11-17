#include "BasicLighting.h"

void Amba::DirLight::render(Shader* shader)
{
	shader->SetUniform3f("dirLight.direction", dir);
	shader->SetUniform4f("dirLight.ambient", ambient);
	shader->SetUniform4f("dirLight.diffuse", diffuse);
	shader->SetUniform4f("dirLight.specular", specular);
}

void Amba::PointLight::render(Shader* shader, int idx)
{
	std::string name = "pointLights[" + std::to_string(idx) + "]";
	shader->SetUniform3f(name + ".position", position);

	shader->SetUniform1f(name + ".k0", k0);
	shader->SetUniform1f(name + ".k1", k1);
	shader->SetUniform1f(name + ".k2", k2);

	shader->SetUniform4f(name + ".ambient", ambient);
	shader->SetUniform4f(name + ".diffuse", diffuse);
	shader->SetUniform4f(name + ".specular", specular);
}

void Amba::SpotLight::render(Shader* shader, int idx)
{
	std::string name = "spotLights[" + std::to_string(idx) + "]";
	shader->SetUniform3f(name + ".position", position);
	shader->SetUniform3f(name + ".direction", direction);

	shader->SetUniform1f(name + ".cutOff", cutOff);
	shader->SetUniform1f(name + ".outerCutOff", outerCutOff);

	shader->SetUniform1f(name + ".k0", k0);
	shader->SetUniform1f(name + ".k1", k1);
	shader->SetUniform1f(name + ".k2", k2);

	shader->SetUniform4f(name + ".ambient", ambient);
	shader->SetUniform4f(name + ".diffuse", diffuse);
	shader->SetUniform4f(name + ".specular", specular);
}
