#pragma once

#include <glm/glm.hpp>
#include <engine/renderer/dataTypes.h>
#include <engine/material/Material.h>
#include <engine/renderer/Texture.h>

#include <engine/renderer/Buffers.h>

#include <vector>



/* ======================
	COMPONENTS
====================== */

// default vertex layout

struct MeshComponent
{
	std::vector<Amba::Vertex> m_Vertices;
	std::vector<unsigned int> m_Indices;
	Amba::Material m_Material;
	Amba::Texture m_Texture; // for now only one texture

	float m_Size;

	Amba::VertexBufferLayout layout;
};

struct TransformComponent
{
	glm::vec3 m_Position = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 m_Rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 m_Scale = glm::vec3(1.0f, 1.0f, 1.0f);
	float m_Angle;
};

struct RendererComponent
{

};

struct CollisionComponent
{

};

struct AudioComponent
{

};

struct BioAttributes
{

};





