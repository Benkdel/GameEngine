#include "components.h"

#include <engine/physics/Collision.h>


IntersectData ColliderComponent::Intersect(const ColliderComponent& other) const
{
	/* ============ Sphere collisions ============== */

	if (m_Type == TYPE_SPHERE && other.GetType() == TYPE_SPHERE)
	{
		SphereCollider* self	= (SphereCollider*)this;
		return self->Intersect((SphereCollider&)other);
	}

	if (m_Type == TYPE_SPHERE && other.GetType() == TYPE_AAB)
	{
		SphereCollider* self = (SphereCollider*)this;
		return self->IntersectAAB((AABCollider&)other);
	}

	if (m_Type == TYPE_SPHERE && other.GetType() == TYPE_PLANE)
	{
		SphereCollider* self = (SphereCollider*)this;
		return self->IntersectPlane((PlaneCollider&)other);
	}

	/* ============ AAB collisions ============== */
	
	if (m_Type == TYPE_AAB && other.GetType() == TYPE_AAB)
	{
		AABCollider* self = (AABCollider*)this;
		return self->Intersect((AABCollider&)other);
	}

	if (m_Type == TYPE_AAB && other.GetType() == TYPE_SPHERE)
	{
		AABCollider* self		= (AABCollider*)this;
		SphereCollider& _other	= (SphereCollider&)other;
		return _other.IntersectAAB((AABCollider&)self);
	}

	if (m_Type == TYPE_AAB && other.GetType() == TYPE_PLANE)
	{
		// use method from sphere collider class
		AABCollider* self = (AABCollider*)this;
		return self->IntersectPlane((PlaneCollider&)other);
	}


	/* ============ Plane collision ============== */
	
	if (m_Type == TYPE_PLANE && other.GetType() == TYPE_PLANE)
	{
		PlaneCollider* self = (PlaneCollider*)this;
		return self->Intersect((PlaneCollider&)other);
	}

	if (m_Type == TYPE_PLANE && other.GetType() == TYPE_SPHERE)
	{
		// use method from sphere collider class
		PlaneCollider* self		= (PlaneCollider*)this;
		SphereCollider& _other	= (SphereCollider&)other;
		return _other.IntersectPlane((PlaneCollider&)self);
	}

	if (m_Type == TYPE_PLANE && other.GetType() == TYPE_AAB)
	{
		// use method from sphere collider class
		PlaneCollider* self = (PlaneCollider*)this;
		AABCollider& _other = (AABCollider&)other;
		return _other.IntersectPlane((PlaneCollider&)self);
	}

	return IntersectData(false, glm::vec3(0.0f));
}
