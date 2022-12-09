#pragma once

#include <glm/glm.hpp>
#include <engine/dataTypes.h>
#include <engine/material/Material.h>
#include <engine/renderer/Texture.h>
#include <engine/model/Model.h>
#include <engine/io/Camera.h>

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


class TagComponent : public Components
{
public:

	TagComponent() 
		: m_Tag("Undefined") {}

	std::string m_Tag;
};


class MeshComponent : public Components
{
public:
	MeshComponent() = default;

	std::vector<Amba::Vertex>		m_Vertices;
	std::vector<unsigned int>		m_Indices;
	Amba::Material*					p_Material = nullptr;
	std::vector<Amba::Texture*>		m_Textures;

	Amba::VertexBufferLayout layout;

	Amba::Shader* p_Shader = nullptr;
};


class TransformComponent : public Components
{
public:
	TransformComponent() = default;

	int m_CurrentCell = -1;
	int m_IndexInCell = -1;

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

	void UpdateRotAngle(float radians)
	{
		m_Radians = radians;
	}

	void SetTransformMatrix(glm::mat4 mat4)
	{
		m_TransformMatrix = mat4;
	}

	inline glm::vec3 GetPosition() { return m_Position; };
	inline glm::vec3 GetRotationAxis() { return m_RotationAxis; };
	inline glm::vec3 GetScale() { return m_Scale; };
	inline glm::mat4 GetTrsMatrix() { return m_TransformMatrix; };
	inline float GetRotAngle() { return m_Radians; };

	// function for now only should be called from colliders
	bool IsTransformRequired()
	{ 
		bool temp = m_TransformRequired;
		m_TransformRequired = false;
		return temp;
	};

private:

	glm::vec3 m_Position		= glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 m_RotationAxis	= glm::vec3(1.0f, 0.0f, 0.0f);
	glm::vec3 m_Scale			= glm::vec3(1.0f, 1.0f, 1.0f);

	float m_Radians = 0.0f;

	glm::mat4 m_TransformMatrix = glm::mat4(1.0f);

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

	// should this go to integrate?
	void ApplyCollisionResults(float impulse, glm::vec3 normal, EntityId other = -1);

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

	std::vector<EntityId> m_RecentCollisions;
	
	bool m_EntGravityActive = true;
	

};

class CameraComponent : public Components
{
public:

	CameraComponent()
		: m_Camera(new Amba::Camera()) {}



	Amba::Camera* GetCamera() { return m_Camera; };

private:
	Amba::Camera* m_Camera;

};


class ControllerComponent : public Components
{
public:

	ControllerComponent() = default;



private:


};

class AudioComponent : public Components
{
public:
	AudioComponent() = default;

};




