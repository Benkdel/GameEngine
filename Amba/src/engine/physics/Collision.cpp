#include "Collision.h"

#include <engine/scene/Scene.h>

// ======================================================================================
// ====================== sphere collider implementation ================================
// ======================================================================================

static glm::vec3 GetNormVector(glm::vec3 vec)
{
	if (vec.x == 0 && vec.y == 0 && vec.z == 0)
		return glm::vec3(1.0f, 0.0f, 0.0f);
	return glm::normalize(vec);
}


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
	
	direction = GetNormVector(direction);

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

	glm::vec3 collNorm = GetNormVector(m_Center - _other.GetCenter());
	
	return IntersectData(distance < m_Radius, direction, collNorm, otherEnt);
}

IntersectData SphereCollider::IntersectPlane(ColliderComponent& other, EntityId otherEnt) const
{
	/*PlaneCollider& _other = (PlaneCollider&)other;
	float distanceFromCenter = glm::dot(_other.GetNormal(), m_Center) + _other.GetDistance();
	distanceFromCenter *= (distanceFromCenter < 0) ? -1 : 1;

	float distanceFromSphere = distanceFromCenter - m_Radius;

	return IntersectData(distanceFromSphere < 0, _other.GetNormal() * distanceFromSphere, _other.GetNormal(), otherEnt);
	*/
	return IntersectAAB(other, otherEnt);
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
		matrix = glm::scale(matrix, tsr->GetScale());
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
// check why positions go crazy when objects are on top of each other (something unrealistic but could be a input)
IntersectData AABCollider::Intersect(ColliderComponent& other, EntityId otherEnt)
{
	AABCollider& _other = (AABCollider&)other;

	glm::vec3 distance1 = _other.GetMinExtents() - m_MaxExtents;
	glm::vec3 distance2 = m_MinExtents - _other.GetMaxExtents();
	glm::vec3 direction = glm::max(distance1, distance2);

	float maxDistance = direction.x;
	for (int i = 0; i < 3; i++)
		maxDistance = (direction[i] > maxDistance) ? direction[i] : maxDistance;

	glm::vec3 collNorm = GetNormVector(m_Center - _other.GetCenter());

	return IntersectData(maxDistance < 0, direction, collNorm, otherEnt);
}

IntersectData AABCollider::IntersectPlane(ColliderComponent& other, EntityId otherEnt)
{
	// TODO: IMPLEMENT - for now trying to use same logic as AAB vs AAB
	//AB_WARN("AAB intersect Plane method no implemented yet!"); 
	return Intersect(other, otherEnt);

	//return IntersectData(false, glm::vec3(0.0f), glm::vec3(0.0f), -1);
}



// ======================================================================================
// ====================== Plane Collider Implementation =================================
// ======================================================================================



PlaneCollider::PlaneCollider()
	: ColliderComponent(ColliderComponent::TYPE_PLANE) {}


void PlaneCollider::InitCollider(MeshComponent* mesh, TransformComponent* tsr)
{
	m_Center = tsr->GetPosition();

	glm::vec3 min = mesh->m_Vertices[0].v_Position;
	glm::vec3 max = mesh->m_Vertices[0].v_Position;

	// get first normal for now -- temp
	m_Normal = mesh->m_Vertices[0].v_Normals;

	for (auto& vec : mesh->m_Vertices)
	{
		float dist1 = (min.x - vec.v_Position.x + min.y - vec.v_Position.y + min.z - vec.v_Position.z);
		float dist2 = (max.x - vec.v_Position.x + max.y - vec.v_Position.y + max.z - vec.v_Position.z);

		min = (dist1 > 0) ? vec.v_Position : min;
		max = (dist2 < 0) ? vec.v_Position : max;
	}
	m_MinExtents = min;
	m_MaxExtents = max;
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


