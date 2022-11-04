#pragma once

#include <glm/glm.hpp>
#include <engine/renderer/dataTypes.h>
#include <engine/material/Material.h>
#include <engine/renderer/Texture.h>
#include <engine/model/Model.h>

#include <engine/renderer/Buffers.h>

#include <vector>


/* ======================
	COMPONENTS
====================== */

class Components
{
public:
	virtual ~Components() {};
};


struct MeshComponent : public Components
{
	std::vector<Amba::Vertex>	m_Vertices;
	std::vector<unsigned int>	m_Indices;
	Amba::Material*				p_Material;
	Amba::Texture*				p_Texture; // for now only one texture

	Amba::VertexBufferLayout layout;
};


struct TransformComponent : public Components
{
	glm::vec3 m_Position = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 m_Rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 m_Scale = glm::vec3(1.0f, 1.0f, 1.0f);
	float m_Angle = 90.0f;
	float m_Size = 1.0f;
	
};

struct CollisionComponent : public Components
{

};

struct AudioComponent : public Components
{

};




