#include "components.h"

#include <engine/physics/Collision.h>
#include <engine/physics/Physics.h>



IntersectData ColliderComponent::Intersect(const ColliderComponent& other, EntityId otherEnt) const
{
	/* ============ Sphere collisions ============== */

	if (m_Type == TYPE_SPHERE && other.GetType() == TYPE_SPHERE)
	{
		SphereCollider* self	= (SphereCollider*)this;
		return self->Intersect((SphereCollider&)other, otherEnt);
	}

	if (m_Type == TYPE_SPHERE && other.GetType() == TYPE_AAB)
	{
		SphereCollider* self = (SphereCollider*)this;
		return self->IntersectAAB((AABCollider&)other, otherEnt);
	}

	if (m_Type == TYPE_SPHERE && other.GetType() == TYPE_PLANE)
	{
		SphereCollider* self = (SphereCollider*)this;
		return self->IntersectPlane((PlaneCollider&)other, otherEnt);
	}

	/* ============ AAB collisions ============== */
	
	if (m_Type == TYPE_AAB && other.GetType() == TYPE_AAB)
	{
		AABCollider* self = (AABCollider*)this;
		return self->Intersect((AABCollider&)other, otherEnt);
	}

	if (m_Type == TYPE_AAB && other.GetType() == TYPE_SPHERE)
	{
		AABCollider* self		= (AABCollider*)this;
		SphereCollider& _other	= (SphereCollider&)other;
		return _other.IntersectAAB((AABCollider&)self, otherEnt);
	}

	if (m_Type == TYPE_AAB && other.GetType() == TYPE_PLANE)
	{
		// use method from sphere collider class
		AABCollider* self = (AABCollider*)this;
		return self->IntersectPlane((PlaneCollider&)other, otherEnt);
	}


	/* ============ Plane collision ============== */
	
	if (m_Type == TYPE_PLANE && other.GetType() == TYPE_PLANE)
	{
		PlaneCollider* self = (PlaneCollider*)this;
		return self->Intersect((PlaneCollider&)other, otherEnt);
	}

	if (m_Type == TYPE_PLANE && other.GetType() == TYPE_SPHERE)
	{
		// use method from sphere collider class
		PlaneCollider* self		= (PlaneCollider*)this;
		SphereCollider& _other	= (SphereCollider&)other;
		return _other.IntersectPlane((PlaneCollider&)self, otherEnt);
	}

	if (m_Type == TYPE_PLANE && other.GetType() == TYPE_AAB)
	{
		// use method from sphere collider class
		PlaneCollider* self = (PlaneCollider*)this;
		AABCollider& _other = (AABCollider&)other;
		return _other.IntersectPlane((PlaneCollider&)self, otherEnt);
	}

	return IntersectData(false, glm::vec3(0.0f), glm::vec3(0.0f));
}


// =========================== Physics Component Implementations ==============================

void PhysicsComponent::CalculateNetForce(glm::vec3 collisionForces)
{
	// stores final net force in member variable to be used later

	
	/*
	1)
		add user defined force
		--------------------------------------------------
	*/
	glm::vec3 force = m_AppliedForce;

	
	
	/*
	2)
		add gravity if activated
		---------------------------------------------------
	*/
	if (Amba::Physics::IsGravityOn() && m_EntGravityActive)
	{
		force += Amba::Physics::GetGravity();
	}

	/*
		add forces resulting from collision or restrictions
		---------------------------------------------------
	*/
	force += collisionForces;


	/*
		save force
		----------------------------------------------------
	*/
	m_NetForce = force;
}


void PhysicsComponent::Integrate(MeshComponent* mesh, TransformComponent* tsr, float dt)
{	
	CalculateNetForce();

	glm::vec3 acceleration = glm::vec3(0.0f);

	acceleration += m_NetForce / m_Mass;

	m_Velocity += acceleration;
	tsr->UpdatePosition(tsr->GetPosition() + m_Velocity * dt);
	
	m_NetForce = glm::vec3(0.0f);
}

void PhysicsComponent::SolveCollision(float impulse, glm::vec3 normal, EntityId other)
{
	if ((other >> 32) != -1)
	{
		std::vector<EntityId>::iterator ent = std::find(m_RecentCollisions.begin(), m_RecentCollisions.end(), other);
		if (ent != m_RecentCollisions.end())
		{
			// ent found, skip applyng force until next iteration
			return;
		}
		else {
			// if not found, then added to avoid next iteration
			m_RecentCollisions.push_back(other);
		}
	}

	m_Velocity = m_Velocity + (impulse / m_Mass) * normal;

	// reset applied force to 0 after collision
	m_AppliedForce = glm::vec3(0.0f);
}

