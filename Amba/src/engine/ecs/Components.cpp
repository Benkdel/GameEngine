#include "components.h"

#include <engine/physics/Collision.h>


IntersectData ColliderComponent::Intersect(const ColliderComponent& other) const
{
	/* ============ Sphere collision ============== */

	if (m_Type == TYPE_SPHERE && other.GetType() == TYPE_SPHERE)
	{
		SphereCollider* self	= (SphereCollider*)this;
		return self->Intersect((SphereCollider&)other);
	}

	//AB_WARN("Collisions not implemented between specified colliders");

	return IntersectData(false, 0);
}
