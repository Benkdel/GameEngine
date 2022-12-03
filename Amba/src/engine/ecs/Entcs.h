#pragma once

#include <engine/ecs/Scene.h>


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

		// check this - method only to be used by copy entity
		Entity(Scene* scene, EntityId id)
		{
			p_Scene = scene;
			m_EntId = id;
		}

		~Entity()
		{
			// Im not destroying this because when class goes out of scope I still have
			// the EntityId types in the scene class

			// Destroy();
		}

		Entity CopyEntity(std::string tag = "undefined")
		{
			EntityId newEnt = p_Scene->CopyEntity(m_EntId);
			return Entity(p_Scene, newEnt);
		}

		void Destroy()
		{
			p_Scene->DestroyEntity(m_EntId);
			p_Scene = nullptr;
		}

		template<typename T>
		T* AddComponent()
		{
			return p_Scene->AddComponent<T>(m_EntId);
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

		inline EntityId GetEntId() { return m_EntId; };
		inline Scene* GetScene() { return p_Scene; };

		bool InitCollider()
		{
			int componentID = GetColliderTypeIndex(m_EntId, p_Scene);

			if (componentID < 0)
				return false;

			ComponentMask entMask = p_Scene->m_Entities[GetEntityIndex(m_EntId)].mask;

			if (!entMask.test(componentID))
				return false;

			ColliderComponent* collider = p_Scene->GetComponentWithId<ColliderComponent>(m_EntId, componentID);

			MeshComponent* mesh = GetComponent<MeshComponent>();
			TransformComponent* tsr = GetComponent<TransformComponent>();

			collider->InitCollider(mesh, tsr);

			collider = nullptr;
			mesh = nullptr;
			tsr = nullptr;
		}


	private:
		EntityId m_EntId;
		Scene* p_Scene;
	};

}


