#pragma once

#include <engine/ecs/Scene.h>

// to reduced radius a bit
#define SPHERE_COLLIDER_ADJUST 0.88f


static float SquaredDistance(glm::vec3& vec1, glm::vec3& vec2)
{
	// not using sqr root for comparison
	return (vec1.x - vec2.x) * (vec1.x - vec2.x) +
		(vec1.y - vec2.y) * (vec1.y - vec2.y) +
		(vec1.z - vec2.z) * (vec1.z - vec2.z);
}


namespace Amba {
		
	class Entity
	{
	public:
		
		Entity() = default;

		Entity(Scene* scene)
		{
			p_Scene = scene;
			m_EntId = scene->CreateEntity();
		}

		~Entity()
		{
			//Destroy();
		}

		void Destroy()
		{
			p_Scene->DestroyEntity(m_EntId);
			p_Scene = nullptr;
		}

		template<typename T>
		void AddComponent()
		{
			p_Scene->AddComponent<T>(m_EntId);
		}

		template<typename T>
		T* GetComponent()
		{
			return p_Scene->GetComponent<T>(m_EntId);
		}

		template<typename T>
		void RemoveComponent()
		{
			p_Scene->RemoveComponent(m_EntId);
		}

		EntityId m_EntId;
		Scene* p_Scene;

		void InitCollider()
		{
			// iterate and get max and mid point se we can set up radius
			MeshComponent* mesh = GetComponent<MeshComponent>();

			glm::vec3& center = GetComponent<TransformComponent>()->m_Position;

			float dist = 0.0f;

			for (auto& vec : mesh->m_Vertices)
			{
				float newDist = SquaredDistance(center, vec.v_Position);
				dist = (newDist > dist) ? newDist : dist;
			}

			// calculate actual distance, sqroot
			dist = sqrt(dist);

			//AB_INFO("Sphere radius: {0} ", dist);

			CollisionComponent* collision = GetComponent<CollisionComponent>();

			collision->m_Radius = dist * SPHERE_COLLIDER_ADJUST;

			collision = nullptr;
			mesh = nullptr;
		}

	private:

		
	};

}


