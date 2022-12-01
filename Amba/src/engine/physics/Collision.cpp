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

	m_Center = tsr->GetPosition();

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

IntersectData SphereCollider::Intersect(ColliderComponent& other, EntityId otherEnt) const
{
	SphereCollider& _other = (SphereCollider&)other;
	
	float radiusDistance = m_Radius + _other.GetRadius();
	glm::vec3 direction = _other.GetCenter() - m_Center;
	float centerDistance = glm::length(direction);
	
	direction = glm::normalize(direction);

	float distance = centerDistance - radiusDistance;

	return IntersectData(distance < 0, direction * distance, direction, otherEnt);
}

IntersectData SphereCollider::IntersectAAB(ColliderComponent& other, EntityId otherEnt) const
{
	AABCollider& _other = (AABCollider&)other;

	glm::vec3 distance1 = _other.GetMinExtents() - m_Center;
	glm::vec3 distance2 = m_Center - _other.GetMaxExtents();
	glm::vec3 direction = glm::max(distance1, distance2);

	float distance = glm::length(direction);

	glm::vec3 collNorm = glm::normalize(m_Center - _other.GetCenter());
	
	return IntersectData(distance < m_Radius, direction, collNorm, otherEnt);
}

IntersectData SphereCollider::IntersectPlane(ColliderComponent& other, EntityId otherEnt) const
{
	PlaneCollider& _other = (PlaneCollider&)other;
	float distanceFromCenter = glm::dot(_other.GetNormal(), m_Center) + _other.GetDistance();
	distanceFromCenter *= (distanceFromCenter < 0) ? -1 : 1;

	float distanceFromSphere = distanceFromCenter - m_Radius;

	return IntersectData(distanceFromSphere < 0, _other.GetNormal() * distanceFromSphere, _other.GetNormal(), otherEnt);
}


// ======================================================================================
// ====================== AAB Collider Implementation ===================================
// ======================================================================================


AABCollider::AABCollider()
	: ColliderComponent(ColliderComponent::TYPE_AAB) {}

void AABCollider::InitCollider(MeshComponent* mesh, TransformComponent* tsr)
{
	m_Center = tsr->GetPosition();

	glm::vec3 min = glm::vec3(0.0f);
	glm::vec3 max = glm::vec3(0.0f);

	for (auto& vec : mesh->m_Vertices)
	{
		// transform vertex
		glm::mat4 matrix = glm::mat4(1.0f);
		matrix = glm::translate(matrix, m_Center);
		matrix = glm::scale(matrix, glm::vec3(tsr->m_Size));
		glm::vec4 v4 = matrix * glm::vec4(vec.v_Position, 1.0f);

		glm::vec3 tsrVertexPosition = glm::vec3(v4.x, v4.y, v4.z);

		float dist1 = (min.x - tsrVertexPosition.x + min.y - tsrVertexPosition.y + min.z - tsrVertexPosition.z);
		float dist2 = (max.x - tsrVertexPosition.x + max.y - tsrVertexPosition.y + max.z - tsrVertexPosition.z);

		min = (dist1 > 0) ? tsrVertexPosition : min;
		max = (dist2 < 0) ? tsrVertexPosition : max;
	}

	m_MinExtents = min;
	m_MaxExtents = max;
}

IntersectData AABCollider::Intersect(ColliderComponent& other, EntityId otherEnt)
{
	AABCollider& _other = (AABCollider&)other;

	glm::vec3 distance1 = _other.GetMinExtents() - m_MaxExtents;
	glm::vec3 distance2 = m_MinExtents - _other.GetMaxExtents();
	glm::vec3 direction = glm::max(distance1, distance2);

	float maxDistance = direction.x;
	for (int i = 0; i < 3; i++)
		maxDistance = (direction[i] > maxDistance) ? direction[i] : maxDistance;

	glm::vec3 collNorm = glm::normalize(m_Center - _other.GetCenter());

	return IntersectData(maxDistance < 0, direction, collNorm, otherEnt);
}

IntersectData AABCollider::IntersectPlane(ColliderComponent& other, EntityId otherEnt)
{
	// TODO: IMPLEMENT - for now trying to use same logic as AAB vs AAB
	//AB_WARN("AAB intersect Plane method no implemented yet!");

	return IntersectData(false, glm::vec3(0.0f), glm::vec3(0.0f), -1);
}



// ======================================================================================
// ====================== Plane Collider Implementation =================================
// ======================================================================================



PlaneCollider::PlaneCollider()
	: ColliderComponent(ColliderComponent::TYPE_PLANE) {}


void PlaneCollider::InitCollider(MeshComponent* mesh, TransformComponent* tsr)
{
	// IMPLEMENT INIT COLLIDER!!
	AB_WARN("Plane collider Init not implemented!");
}

PlaneCollider::PlaneCollider(glm::vec3 normal, float distance)
	: m_Normal(normal), m_Distance(distance),
	ColliderComponent(ColliderComponent::TYPE_PLANE) {}

PlaneCollider PlaneCollider::Normalize() const
{
	float magnitude = glm::length(m_Normal);

	return PlaneCollider(m_Normal / magnitude, m_Distance / magnitude);
}

IntersectData PlaneCollider::Intersect(ColliderComponent& other, EntityId otherEnt)
{
	PlaneCollider& _other = (PlaneCollider&)other;

	// TODO: test collision
	//AB_WARN("Collision between Plane and Plane not yet implemented!");

	return IntersectData(false, glm::vec3(0.0f), glm::vec3(0.0f), otherEnt);
}


