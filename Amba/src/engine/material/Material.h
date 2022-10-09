#pragma once

#include <engine/material/Shader.h>

#include <glad/glad.h>
#include <glm/glm.hpp>

namespace Amba {

	class Material
	{
	public:

		Material();

		void SetColorProperties(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
		void SetTextures(unsigned int diffuseTex, unsigned int specularTex);
	
		~Material();

		void SetMaterial(Shader& shader);

	private:

		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
		unsigned int diffuseTex;
		unsigned int specularTex;

	};
};
