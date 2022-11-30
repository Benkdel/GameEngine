#include "Physics.h"

#include <engine/ecs/Scene.h>


namespace Amba {

	glm::vec3 Physics::m_Gravity = glm::vec3(0.0f, -9.8f, 0.0f);
	bool Physics::m_IsGravityOn = false;

	void Physics::ApplyMotion(Scene* scene, float dt)
	{
		for (EntityId ent : SceneView<TransformComponent, PhysicsComponent>(scene))
		{
			scene->GetComponent<PhysicsComponent>(ent)
						->Integrate(scene->GetComponent<MeshComponent>(ent),
									scene->GetComponent<TransformComponent>(ent), dt);
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
				AB_INFO("DIRECTION: {0}, {1}, {2}", intersect.GetDirection().x, intersect.GetDirection().y, 
					intersect.GetDirection().z);

				//glm::vec3 overlap = intersect.GetDirection() * 0.5f;

				//scene->GetComponent<TransformComponent>(ent)->m_Position += overlap;
				//scene->GetComponent<TransformComponent>(intersect.GetOtherEnt())->m_Position -= overlap;
			
				// lets try momentum equations
				// momentum is conserved assuming there is no friction
				// elastic collision
				
				glm::vec3 entV0 = scene->GetComponent<PhysicsComponent>(ent)->GetVelocity();
				glm::vec3 otherEntV0 = scene->GetComponent<PhysicsComponent>(intersect.GetOtherEnt())->GetVelocity();
			
				float entMass = scene->GetComponent<PhysicsComponent>(ent)->m_Mass;
				float otherEntMass = scene->GetComponent<PhysicsComponent>(intersect.GetOtherEnt())->m_Mass;

				glm::vec3 relVelocity = entV0 - otherEntV0;

				// I know objects will collide in X and normal vector pointing to ent is x negative
				// implement getting real normal from IntersectData
				glm::vec3 normal = /*glm::vec3(-1.0f, 0.0f, 0.0f);*/ intersect.GetCollisionNorm();

				float e = 1.0f; // 100% elastic

				float numerator		= glm::dot(- 1 * (1 + e) * relVelocity, normal);
				float denominator	= glm::dot(normal, normal * (1 / entMass + 1 / otherEntMass)); // 1 instead of dot normal normal, because is the same when unit length?

				float impulse = numerator / denominator;

				AB_INFO("Impulse: {0}", impulse);

				scene->GetComponent<PhysicsComponent>(ent)->SolveCollision(impulse, normal);
				scene->GetComponent<PhysicsComponent>(intersect.GetOtherEnt())->SolveCollision(-impulse, normal);
			
			}
		}
	}

	IntersectData Physics::CheckForCollision(EntityId id, Scene* scene)
	{
		int componentID = GetColliderTypeIndex(id, scene);

		if (componentID < 0)
			return IntersectData(false, glm::vec3(0.0f), glm::vec3(0.0f));
	
		ComponentMask entMask = scene->m_Entities[GetEntityIndex(id)].mask;

		if (!entMask.test(componentID))
			return IntersectData(false, glm::vec3(0.0f), glm::vec3(0.0f));

		TransformComponent* tsr = scene->GetComponent<TransformComponent>(id);
		Cell& cell = scene->m_Spatial2DGrid->GetCell(tsr->m_Position);
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
					IntersectData intersect = collider->Intersect(*otherCollider, other);

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
			IntersectData intersect = collider->Intersect(*otherCollider, plane);

			if (intersect.GetDoesIntersect())
			{
				return intersect;
			}
		}

		return IntersectData(false, glm::vec3(0.0f), glm::vec3(0.0f));
	}



}



