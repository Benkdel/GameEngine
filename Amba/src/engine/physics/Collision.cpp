#include "Collision.h"

#include <engine/ecs/Scene.h>

// sphere collider implementation

SphereCollider::SphereCollider()
	: ColliderComponent(ColliderComponent::TYPE_SPHERE) {}

IntersectData SphereCollider::Intersect(SphereCollider& other) const
{
	float radiusDistance = m_Radius + other.GetRadius();
	float centerDistance = glm::distance(other.GetCenter(), m_Center);

	float distance = centerDistance - radiusDistance;

	return IntersectData(distance < 0, distance);
}

void SphereCollider::SetColliderParameters(float radius, glm::vec3 center)
{
	m_Radius = radius;
	m_Center = center;
}

// AAB collider implementation

AABCollider::AABCollider()
	: ColliderComponent(ColliderComponent::TYPE_AAB) {}

void AABCollider::CheckForCollision(ColliderComponent& other)
{

}



// Plane collider implementation

PlaneCollider::PlaneCollider()
	: ColliderComponent(ColliderComponent::TYPE_PLANE) {}

void PlaneCollider::CheckForCollision(ColliderComponent& other)
{

}


