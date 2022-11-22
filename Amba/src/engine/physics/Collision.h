#pragma once

#include <engine/ecs/components.h>


class SphereCollider : public ColliderComponent
{
public:
	SphereCollider();

	void SetColliderParameters(float radius, glm::vec3 center);

	IntersectData Intersect(SphereCollider& other) const;

	inline float GetRadius()		const { return m_Radius; };
	inline glm::vec3 GetCenter()	const { return m_Center; };

	virtual inline void TransformCollider(TransformComponent* tsr) { m_Center = tsr->m_Position; };

private:

	float m_Radius = -1.0f;
	glm::vec3 m_Center = glm::vec3(0.0f);
};


class AABCollider : public ColliderComponent
{
public:

	AABCollider();

	// to be implemented
	void SetColliderParameters() {};

	void CheckForCollision(ColliderComponent& other);

	virtual inline void TransformCollider(TransformComponent* tsr) { m_Center = tsr->m_Position; };

private:

	glm::vec3 m_Center = glm::vec3(0.0f);

};



class PlaneCollider : public ColliderComponent
{
public:
	PlaneCollider();

	// to be implemented
	void SetColliderParameters() {};

	void CheckForCollision(ColliderComponent& other);

	virtual inline void TransformCollider(TransformComponent* tsr) { m_Center = tsr->m_Position; };

private:

	glm::vec3 m_Center = glm::vec3(0.0f);

};
