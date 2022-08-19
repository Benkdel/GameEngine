#include "Material.h"

namespace Amba {
	
	Material::Material()
	{
	}

	void Material::SetColorProperties(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
	{
		this->ambient = ambient;
		this->diffuse = diffuse;
		this->specular = specular;
	}
	
	void Material::SetTextures(unsigned int diffuseTex, unsigned int specularTex)
	{
		this->diffuseTex = diffuseTex;
		this->specularTex = specularTex;
	}

	Material::~Material()
	{
	}

	void Material::SetMaterial(Shader& shader)
	{

	}


};

