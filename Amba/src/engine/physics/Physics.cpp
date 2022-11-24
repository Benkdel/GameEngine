#include "Physics.h"

#include <engine/ecs/Scene.h>


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
			// check 
			IntersectData intersect = CheckForCollision(ent, scene);
			
			if (intersect.GetDoesIntersect())
			{
				AB_INFO("COLLISION - Entity: {0} || Dist: {1} ", ent, intersect.GetDistance());

				glm::vec3 direction = glm::normalize(intersect.GetDirection());

				scene->GetComponent<PhysicsComponent>(ent)->m_Velocity = glm::reflect(
					scene->GetComponent<PhysicsComponent>(ent)->m_Velocity, direction);
			}
		}
	}

	IntersectData Physics::CheckForCollision(EntityId id, Scene* scene)
	{
		int componentID = GetColliderTypeIndex(id, scene);

		if (componentID < 0)
			return IntersectData(false, glm::vec3(0.0f));
	
		ComponentMask entMask = scene->m_Entities[GetEntityIndex(id)].mask;

		if (!entMask.test(componentID))
			return IntersectData(false, glm::vec3(0.0f));

		TransformComponent* tsr = scene->GetComponent<TransformComponent>(id);
		Cell& cell = scene->m_Spatial2DGrid.GetCell(tsr->m_Position);
		ColliderComponent* collider = scene->GetComponentWithId<ColliderComponent>(id, componentID);

		// update collider position (center)
		collider->TransformCollider(tsr);

		// check entities in cell
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

		// check planes
		for (EntityId plane : SceneView<PlaneCollider>(scene))
		{
			PlaneCollider* otherCollider = scene->GetComponent<PlaneCollider>(plane);
			IntersectData intersect = collider->Intersect(*otherCollider);

			if (intersect.GetDoesIntersect())
			{
				return intersect;
			}
		}

		return IntersectData(false, glm::vec3(0.0f));
	}



}



