#pragma once

#include <engine/material/Shader.h>
#include <engine/lighting/BasicLighting.h>
#include <engine/lighting/PbrLighting.h>

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

		glm::vec3 m_Albedo;
		
		float m_Ao;
		float m_Metallic;
		float m_Roughness;


	private:

		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
		unsigned int diffuseTex;
		unsigned int specularTex;

	};
};
