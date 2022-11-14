#pragma once

#include <glm/glm.hpp>
#include <engine/dataTypes.h>
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

	Amba::Shader* p_Shader;
};


struct TransformComponent : public Components
{
	glm::vec3 m_Position = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 m_Rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 m_Scale = glm::vec3(1.0f, 1.0f, 1.0f);
	float m_Angle = 90.0f;
	float m_Size = 1.0f;

	glm::vec3 m_Velocity = glm::vec3(0.0f);

	int m_CurrentCell = -1;
	int m_IndexInCell = -1;

};

struct CollisionComponent : public Components
{	
	// for now. Sphere only
	float m_Radius = 0.0f;
	glm::vec3 m_Center = glm::vec3(0.0f);

};

struct AudioComponent : public Components
{

};




