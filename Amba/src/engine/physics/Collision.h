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

	inline void TransformCollider(TransformComponent* tsr) { m_Center = tsr->m_Position; };

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
	
	inline void TransformCollider(TransformComponent* tsr) { m_Center = tsr->m_Position; };

	inline glm::vec3 GetMinExtents() { return m_MinExtents; };
	inline glm::vec3 GetMaxExtents() { return m_MaxExtents; };

private:
	glm::vec3 m_MinExtents = glm::vec3(0.0f);
	glm::vec3 m_MaxExtents = glm::vec3(0.0f);
	
	glm::vec3 m_Center = glm::vec3(0.0f);

};



class PlaneCollider : public ColliderComponent
{
public:
	PlaneCollider();

	PlaneCollider(glm::vec3 normal, float distance);

	void InitCollider(MeshComponent* mesh, TransformComponent* tsr);

	IntersectData Intersect(ColliderComponent& other, EntityId otherEnt);

	PlaneCollider Normalize() const;

	virtual inline void TransformCollider(TransformComponent* tsr) { m_Center = tsr->m_Position; };

	inline glm::vec3 GetNormal()	{ return m_Normal; };
	inline float GetDistance()		{ return m_Distance; };
	inline glm::vec3 GetCenter()	{ return m_Center; };

private:
	glm::vec3 m_Normal = glm::vec3(0.0f);
	float m_Distance = 0.0f;
	
	glm::vec3 m_Center = glm::vec3(0.0f);

};
