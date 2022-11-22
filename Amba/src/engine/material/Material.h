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

		Material() = default;


	private:

		

	};



	class PbrMaterial : Material
	{
	public:

		PbrMaterial();


	private:

	};


	// Implement other type of material if necessary later

};
