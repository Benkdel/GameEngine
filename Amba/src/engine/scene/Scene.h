#pragma once

#include <engine/dataTypes.h>

#include <engine/ecs/EntHandler.h>
#include <engine/physics/Collision.h>
#include <engine/physics/Physics.h>

#include <unordered_map>

#include <bitset>
#include <vector>

static int GetColliderTypeIndex(EntityId id, Amba::Scene* scene);

#define EDITOR_CAMERA_TAG "EditorCamera"

namespace Amba {

	// forward declarator
	class Entity;

	class Scene 
	{

	public:
		Scene();

		~Scene() { AB_WARN("Scene destructor called!"); };

		void Update(float dt);
	
		EntityId GetEntityByTag(const std::string& tag);

	public: // camera handler

		void AddCameraObject(Entity* entity, bool primary);
		void SetActiveCamera(Entity* entity);
		void SetActiveCamera(EntityId id);
		void DeleteCamera(Entity* entity);
		Camera* GetActiveCamera();
		inline const ViewPortData& GetViewPortData() const { return m_ViewPortData; };

	public: // templates

		template <typename T>
		bool EntHasComponent(EntityId id)
		{
			return p_EntHandler->EntHasComponent<T>(id);
		}

		template<typename T>
		T* AddComponent(EntityId id)
		{
			return p_EntHandler->AddComponent<T>(id);
		}

		template <typename T>
		T* GetComponentWithId(EntityId id, int componentId)
		{
			return p_EntHandler->GetComponentWithId<T>(id, componentId);
		}

		template<typename T>
		T* GetComponent(EntityId id)
		{
			return p_EntHandler->GetComponent<T>(id);
		}

		template<typename T>
		void RemoveComponent(EntityId id)
		{
			p_EntHandler->RemoveComponent<T>(id);
		}

		void Cleanup();

		Physics* p_PhysicsEngine;
		EntityHandler* p_EntHandler;

	private:
		
		EntityId CreateEntity();
		EntityId CopyEntity(EntityId id);
		void DestroyEntity(EntityId id);

		std::vector<EntityId> m_AvailableCameras;
		EntityId m_ActiveCamera;

		// viewport data
		ViewPortData m_ViewPortData;

		friend class Application;
		friend class Entity;
		friend class Spatial2DGrid;
	};
}

// utils
static int GetColliderTypeIndex(EntityId id, Amba::Scene* scene)
{
	ComponentMask mask = scene->p_EntHandler->m_Entities[scene->p_EntHandler->GetEntityIndex(id)].mask;

	for (int bit = 0; bit < mask.size(); bit++)
	{
		if (mask.test(bit))
		{
			void* component = scene->p_EntHandler->m_ComponentPools[bit]->get(
				scene->p_EntHandler->GetEntityIndex(id));
			
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