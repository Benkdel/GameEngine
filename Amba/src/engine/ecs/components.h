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

	float m_Radians = 0.0f;
	float m_Size = 1.0f;

	int m_CurrentCell = -1.0f;
	int m_IndexInCell = -1.0f;

	void UpdatePosition(glm::vec3 pos)
	{
		m_Position = pos;
		m_TransformRequired = true;
	}

	void UpdateRotationAxis(glm::vec3 rotAxis)
	{
		m_RotationAxis = rotAxis;
		m_TransformRequired = true;
	}

	void UpdateScale(glm::vec3 scale)
	{
		m_Scale = scale;
		m_TransformRequired = true;
	}

	inline glm::vec3 GetPosition() { return m_Position; };
	inline glm::vec3 GetRotationAxis() { return m_RotationAxis; };
	inline glm::vec3 GetScale() { return m_Scale; };

	// function for now only should be called from colliders
	bool IsTransformRequired() 
	{ 
		bool temp = m_TransformRequired;
		m_TransformRequired = false;
		return temp;
	};

private:

	glm::vec3 m_Position		= glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 m_RotationAxis	= glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 m_Scale			= glm::vec3(1.0f, 1.0f, 1.0f);

	bool m_TransformRequired = true;

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
	virtual IntersectData Intersect(const ColliderComponent& other, EntityId otherEnt = -1) const;


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

	void Integrate(MeshComponent* mesh, TransformComponent* tsr, float dt);
	void CalculateNetForce(glm::vec3 collisionForces = glm::vec3(0.0f));

	void SolveCollision(float impulse, glm::vec3 normal);

	void inline ApplyForce(glm::vec3 force)			{ m_AppliedForce = force; };
	void inline IncreaseForce(glm::vec3 force)		{ m_AppliedForce += force; };

	glm::vec3 inline GetForce()				{ return m_AppliedForce; };
	
	float m_Mass = 0.0f;

	inline glm::vec3 GetVelocity()			{ return m_Velocity; };
	inline bool GetEntGravityStatus()		{ return m_EntGravityActive; };

	inline void ToggleEntGravity(bool isActive) { m_EntGravityActive = isActive; };

private:

	glm::vec3 m_Velocity			= glm::vec3(0.0f);
	glm::vec3 m_NetForce			= glm::vec3(0.0f);
	glm::vec3 m_AppliedForce		= glm::vec3(0.0f);

	
	bool m_EntGravityActive = true;
	

};


class AudioComponent : public Components
{
public:
	AudioComponent() = default;

};




