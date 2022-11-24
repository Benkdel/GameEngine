#include "Collision.h"

#include <engine/ecs/Scene.h>

// ======================================================================================
// ====================== sphere collider implementation ================================
// ======================================================================================

SphereCollider::SphereCollider()
	: ColliderComponent(ColliderComponent::TYPE_SPHERE) {}

void SphereCollider::InitCollider(MeshComponent* mesh, TransformComponent* tsr)
{
	float dist = 0.0f;
	// to reduced radius a bit
	float SPHERE_COLLIDER_ADJUST = 0.88f;

	m_Center = tsr->m_Position;

	for (auto& vec : mesh->m_Vertices)
	{
		float newDist = glm::distance(m_Center, vec.v_Position);
		dist = (newDist > dist) ? newDist : dist;
	}

	m_Radius = dist * SPHERE_COLLIDER_ADJUST;
}

void SphereCollider::InitMouseCollider(float radius, glm::vec3 center)
{
	m_Radius = radius;
	m_Center = center;
}

IntersectData SphereCollider::Intersect(ColliderComponent& other) const
{
	SphereCollider& _other = (SphereCollider&)other;
	
	float radiusDistance = m_Radius + _other.GetRadius();
	glm::vec3 direction = _other.GetCenter() - m_Center;
	float centerDistance = glm::length(direction);
	
	direction = glm::normalize(direction);

	float distance = centerDistance - radiusDistance;

	return IntersectData(distance < 0, direction * distance);
}

IntersectData SphereCollider::IntersectAAB(ColliderComponent& other) const
{
	AABCollider& _other = (AABCollider&)other;

	// TODO: test collision

	//AB_WARN("Collision between Sphere and AAB not yet implemented!");

	return IntersectData(false, glm::vec3(0.0f));
}

IntersectData SphereCollider::IntersectPlane(ColliderComponent& other) const
{
	PlaneCollider& _other = (PlaneCollider&)other;
	float distanceFromCenter = glm::dot(_other.GetNormal(), m_Center) + _other.GetDistance();
	distanceFromCenter *= (distanceFromCenter < 0) ? -1 : 1;

	float distanceFromSphere = distanceFromCenter - m_Radius;

	return IntersectData(distanceFromSphere < 0, _other.GetNormal() * distanceFromSphere);
}


// ======================================================================================
// ====================== AAB Collider Implementation ===================================
// ======================================================================================


AABCollider::AABCollider()
	: ColliderComponent(ColliderComponent::TYPE_AAB) {}

void AABCollider::InitCollider(MeshComponent* mesh, TransformComponent* tsr)
{
	m_Center = tsr->m_Position;

	glm::vec3 min = glm::vec3(0.0f);
	glm::vec3 max = glm::vec3(0.0f);

	for (auto& vec : mesh->m_Vertices)
	{
		min = (glm::distance(min, vec.v_Position) > 0) ? vec.v_Position : min;
		max = (glm::distance(max, vec.v_Position) < 0) ? vec.v_Position : max;
	}

	m_MinExtents = min;
	m_MaxExtents = max;
}

IntersectData AABCollider::Intersect(ColliderComponent& other)
{
	AABCollider& _other = (AABCollider&)other;

	glm::vec3 distance1 = _other.GetMinExtents() - m_MaxExtents;
	glm::vec3 distance2 = m_MinExtents - _other.GetMaxExtents();
	glm::vec3 distance = glm::max(distance1, distance2);

	float maxDistance = 0.0f;
	for (int i = 0; i < 3; i++)
		maxDistance = (distance[0] > maxDistance) ? distance[0] : maxDistance;

	return IntersectData(maxDistance > 0, distance);
}

IntersectData AABCollider::IntersectPlane(ColliderComponent& other)
{
	// TODO: IMPLEMENT
	//AB_WARN("Collision between AAB and Plane not yet implemented!");
	return IntersectData(false, glm::vec3(0.0f));
}



// ======================================================================================
// ====================== Plane Collider Implementation =================================
// ======================================================================================



PlaneCollider::PlaneCollider()
	: ColliderComponent(ColliderComponent::TYPE_PLANE) {}


void PlaneCollider::InitCollider(MeshComponent* mesh, TransformComponent* tsr)
{
	m_Normal = mesh->m_Vertices[0].v_Normals; // every normal is the same, just pick first one
	m_Distance = glm::distance(tsr->m_Position, glm::vec3(0.0f));
	m_Center = tsr->m_Position;
}

PlaneCollider::PlaneCollider(glm::vec3 normal, float distance)
	: m_Normal(normal), m_Distance(distance),
	ColliderComponent(ColliderComponent::TYPE_PLANE) {}

PlaneCollider PlaneCollider::Normalize() const
{
	float magnitude = glm::length(m_Normal);

	return PlaneCollider(m_Normal / magnitude, m_Distance / magnitude);
}

IntersectData PlaneCollider::Intersect(ColliderComponent& other)
{
	PlaneCollider& _other = (PlaneCollider&)other;

	// TODO: test collision
	//AB_WARN("Collision between Plane and Plane not yet implemented!");

	return IntersectData(false, glm::vec3(0.0f));
}


