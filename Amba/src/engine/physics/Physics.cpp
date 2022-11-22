#include "Physics.h"

#include <engine/ecs/Scene.h>



// Check if entity has any of the collision derived classes
// return correct component index
static int GetColliderTypeIndex(EntityId id, Amba::Scene* scene)
{
	ComponentMask mask = scene->m_Entities[Amba::GetEntityIndex(id)].mask;

	for (int bit = 0; bit < mask.size(); bit++)
	{
		if (mask.test(bit))
		{	
			void* component = scene->m_ComponentPools[bit]->get(Amba::GetEntityIndex(id));
			SphereCollider* spColl = dynamic_cast<SphereCollider*>((Components*)component);
			AABCollider* aabColl = dynamic_cast<AABCollider*>((Components*)component);
			PlaneCollider* planeColl = dynamic_cast<PlaneCollider*>((Components*)component);

			if (spColl != NULL)			return Amba::GetComponentId<SphereCollider>();
			else if (aabColl != NULL)	return Amba::GetComponentId<AABCollider>();
			else if (planeColl != NULL)	return Amba::GetComponentId<PlaneCollider>();
			else;
		}
	}

	return -1;
}

namespace Amba {

	float Physics::m_Gravity = 9.8f;

	void Physics::ApplyMotion(Scene* scene, float dt)
	{
		for (EntityId ent : SceneView<TransformComponent, PhysicsComponent>(scene))
		{
			TransformComponent* tsr = scene->GetComponent<TransformComponent>(ent);
			PhysicsComponent* physics = scene->GetComponent<PhysicsComponent>(ent);

			tsr->m_Position += physics->m_Velocity * dt;
		}
	}


	void Physics::SolveCollisions(Scene* scene)
	{
		for (EntityId ent : SceneView<TransformComponent, MeshComponent>(scene))
		{
			IntersectData intersect = CheckForCollision(ent, scene);
			if (intersect.GetDoesIntersect())
			{
				AB_INFO("COLLISION - Entity: {0} || Dist: {1} ", ent, intersect.GetDistance());
				// for testing - only works because I know ents are going to collide on x axis
				scene->GetComponent<PhysicsComponent>(ent)->m_Velocity *= -1.0f;
			}
		}

	}

	IntersectData Physics::CheckForCollision(EntityId id, Scene* scene)
	{
		int componentID = GetColliderTypeIndex(id, scene);

		if (componentID < 0)
			return IntersectData(false, 0);
	
		ComponentMask entMask = scene->m_Entities[GetEntityIndex(id)].mask;

		if (!entMask.test(componentID))
			return IntersectData(false, 0);

		TransformComponent* tsr = scene->GetComponent<TransformComponent>(id);
		Cell& cell = scene->m_Spatial2DGrid.GetCell(tsr->m_Position);
		ColliderComponent* collider = scene->GetComponentWithId<ColliderComponent>(id, componentID);

		// update collider position (center)
		collider->TransformCollider(tsr);

		for (auto& other : cell.entities)
		{
			if (id != other)
			{
				// check if other entity has collider component
				ComponentMask otherMask = scene->m_Entities[GetEntityIndex(other)].mask;

				if (otherMask.test(componentID))
				{
					ColliderComponent* otherCollider = scene->GetComponentWithId<ColliderComponent>(other, componentID);
					IntersectData intersect = collider->Intersect(*otherCollider);

					if (intersect.GetDoesIntersect())
					{
						return intersect;
					}
				}
			}
		}
		return IntersectData(false, 0);
	}




}



