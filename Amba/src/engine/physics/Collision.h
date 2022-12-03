#pragma once

#include <engine/ecs/components.h>


class SphereCollider : public ColliderComponent
{
public:

	SphereCollider();

	void InitCollider(MeshComponent* mesh, TransformComponent* tsr);

	// for mouse picker
	void InitMouseCollider(float radius, glm::vec3 center);
		
	IntersectData Intersect(ColliderComponent& other, EntityId otherEnt) const;
	IntersectData IntersectAAB(ColliderComponent& other, EntityId otherEnt) const;
	IntersectData IntersectPlane(ColliderComponent& other, EntityId otherEnt) const;

	inline float GetRadius()		const { return m_Radius; };
	inline glm::vec3 GetCenter()	const { return m_Center; };

	inline void TransformCollider(TransformComponent* tsr) { m_Center = tsr->GetPosition(); };

private:

	float m_Radius = -1.0f;
	glm::vec3 m_Center = glm::vec3(0.0f);

};


class AABCollider : public ColliderComponent
{
public:

	AABCollider();

	void InitCollider(MeshComponent* mesh, TransformComponent* tsr);
	
	IntersectData Intersect(ColliderComponent& other, EntityId otherEnt);
	IntersectData IntersectPlane(ColliderComponent& other, EntityId otherEnt);
	
	void TransformCollider(TransformComponent* tsr)
	{ 
		if (tsr->IsTransformRequired())
			UpdateCollisionVertex(tsr);
	};

	inline glm::vec3 GetMinExtents() { return m_MinExtents; };
	inline glm::vec3 GetMaxExtents() { return m_MaxExtents; };
	inline glm::vec3 GetCenter()	 { return m_Center; };

private:
	glm::vec3 m_MinExtents = glm::vec3(0.0f);
	glm::vec3 m_MaxExtents = glm::vec3(0.0f);
	glm::vec3 m_Normal = glm::vec3(0.0f);
	
	glm::vec3 m_Center = glm::vec3(0.0f);

	void UpdateCollisionVertex(TransformComponent* tsr)
	{
		if (m_Center == tsr->GetPosition())
			return;

		glm::vec3 prevPosition = m_Center;

		m_Center = tsr->GetPosition();

		m_MinExtents += m_Center - prevPosition;
		m_MaxExtents += m_Center - prevPosition;
	}

};



class PlaneCollider : public ColliderComponent
{
public:
	PlaneCollider();

	PlaneCollider(glm::vec3 normal, float distance);

	void InitCollider(MeshComponent* mesh, TransformComponent* tsr);

	IntersectData Intersect(ColliderComponent& other, EntityId otherEnt);

	PlaneCollider Normalize() const;

	void TransformCollider(TransformComponent* tsr)
	{
		tsr->IsTransformRequired();
			UpdateCollisionVertex(tsr);
	};

	inline glm::vec3 GetNormal()	{ return m_Normal; };
	inline glm::vec3 GetMinExtents() { return m_MinExtents; };
	inline glm::vec3 GetMaxExtents() { return m_MaxExtents; };

	inline float GetDistance()		{ return m_Distance; };
	inline glm::vec3 GetCenter()	{ return m_Center; };

private:
	glm::vec3 m_MinExtents = glm::vec3(0.0f);
	glm::vec3 m_MaxExtents = glm::vec3(0.0f);

	glm::vec3 m_Normal = glm::vec3(0.0f);
	float m_Distance = 0.0f;
	
	glm::vec3 m_Center = glm::vec3(0.0f);

	void UpdateCollisionVertex(TransformComponent* tsr)
	{
		if (m_Center == tsr->GetPosition())
			return;

		glm::vec3 prevPosition = m_Center;

		m_Center = tsr->GetPosition();

		m_MinExtents += m_Center - prevPosition;
		m_MaxExtents += m_Center - prevPosition;
	}
};
