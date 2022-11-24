#pragma once

#include <glm/glm.hpp>
#include <engine/dataTypes.h>
#include <engine/material/Material.h>
#include <engine/renderer/Texture.h>
#include <engine/model/Model.h>

#include <engine/renderer/Buffers.h>

#include <engine/dataStructures/SpatialHashGrid.h>

#include <vector>



/* ======================
	COMPONENTS
====================== */

class Components
{
public:
	virtual ~Components() {};
};


class MeshComponent : public Components
{
public:
	MeshComponent() = default;

	std::vector<Amba::Vertex>	m_Vertices;
	std::vector<unsigned int>	m_Indices;
	Amba::Material*				p_Material = nullptr;
	Amba::Texture*				p_Texture = nullptr; // for now only one texture

	Amba::VertexBufferLayout layout;

	Amba::Shader* p_Shader = nullptr;
};


class TransformComponent : public Components
{
public:
	TransformComponent() = default;

	glm::vec3 m_Position = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 m_Rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 m_Scale = glm::vec3(1.0f, 1.0f, 1.0f);

	float m_Angle = 90.0f;
	float m_Size = 1.0f;

	glm::vec3 m_Velocity = glm::vec3(0.0f);

	int m_CurrentCell = -1.0f;
	int m_IndexInCell = -1.0f;

};

// Collider component should be updated if entity changes size
class ColliderComponent : public Components
{	
public:

	enum
	{
		TYPE_SPHERE,
		TYPE_AAB,
		TYPE_PLANE,
		TYPE_SIZE
	};

	ColliderComponent() = default;

	ColliderComponent(int type)
	{
		m_Type = type;
	};
	
	~ColliderComponent() {};
	
	virtual void InitCollider(MeshComponent* mesh, TransformComponent* tsr) {};

	// see if later we can implement template functions or dynamic casting
	virtual IntersectData Intersect(const ColliderComponent& other) const;


	// Use derived classes in physics folder:
	// * SphereCollider
	// * AABCollider
	// * PlaneCollider
	// * etc

	inline int GetType() const { return m_Type; };

	virtual inline void TransformCollider(TransformComponent* tsr) {};

private:
	int m_Type;

};


class PhysicsComponent : public Components
{
public:
	PhysicsComponent() = default;


	glm::vec3 m_Velocity = glm::vec3(0.0f);
	float m_Mass = 0.0f;

private:


};


class AudioComponent : public Components
{
public:
	AudioComponent() = default;

};




