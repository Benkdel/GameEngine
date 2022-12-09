#pragma once

#include <engine/ecs/ComponentPool.h>
#include "components.h"
#include <engine/physics/Collision.h>
#include <engine/physics/Physics.h>

#include <engine/dataStructures/SpatialHashGrid.h>
#include <engine/dataTypes.h>

#include <unordered_map>

#include <bitset>
#include <vector>

const int MAX_COMPONENTS = 32;
typedef std::bitset<MAX_COMPONENTS> ComponentMask;

static int GetColliderTypeIndex(EntityId id, Amba::Scene* scene);

#define EDITOR_CAMERA_TAG "EditorCamera"

namespace Amba {


	// entity handlers
	EntityId CreateEntityId(EntityIndex index, EntityVersion version);
	EntityIndex GetEntityIndex(EntityId id);
	EntityVersion GetEntityVersion(EntityId id);
	bool IsEntityValid(EntityId id);

	// forward declarator
	class Entity;

	class Scene 
	{

	public:
		Scene();

		~Scene() { AB_WARN("Scene destructor called!"); };

		void Update(float dt);

		void AssignEntity(EntityId id);
		std::vector<Cell> GetNearbyCells(glm::vec3 position);
		void FindNearEntities(EntityId id);
	
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
			// get component id for collision and check if entiy has it
			int componentId = GetComponentId<T>();
			ComponentMask comp;
			comp.set(componentId);

			// check if entity in cell has collision component
			if (m_Entities[GetEntityIndex(id)].mask ==
				(m_Entities[GetEntityIndex(id)].mask | comp))
				return true;

			return false;
		}

		template<typename T>
		T* AddComponent(EntityId id)
		{
			AB_ASSERT(IsEntityValid(id), "Invalid EntityId");
			int componentId = GetComponentId<T>();

			if (m_ComponentPools.size() <= componentId)
			{
				m_ComponentPools.resize(componentId + 1, nullptr);
			}

			if (m_ComponentPools[componentId] == nullptr)
			{
				m_ComponentPools[componentId] = new ComponentPool(sizeof(T));
			}

			T* newComponent = new (m_ComponentPools[componentId]->get(GetEntityIndex(id))) T();

			m_Entities[GetEntityIndex(id)].mask.set(componentId);

			return newComponent;
		}

		template <typename T>
		T* GetComponentWithId(EntityId id, int componentId)
		{
			AB_ASSERT(IsEntityValid(id), "Invalid EntityId");
			if (componentId < 0)
				return nullptr;

			if (!m_Entities[GetEntityIndex(id)].mask.test(componentId))
				return nullptr;

			T* component = static_cast<T*>(m_ComponentPools[componentId]->get(GetEntityIndex(id)));
			return component;
		}

		template<typename T>
		T* GetComponent(EntityId id)
		{
			AB_ASSERT(IsEntityValid(id), "Invalid EntityId");
			int componentId = GetComponentId<T>();
			if (!m_Entities[GetEntityIndex(id)].mask.test(componentId))
				return nullptr;
		
			T* component = static_cast<T*>(m_ComponentPools[componentId]->get(GetEntityIndex(id)));
			return component;
		}

		template<typename T>
		void RemoveComponent(EntityId id)
		{
			AB_ASSERT(IsEntityValid(id), "Invalid EntityId");
			if (m_Entities[GetEntityIndex(id)].id != id)
				return;

			int componentId = GetComponentId<T>();
			m_Entities[GetEntityIndex(id)].mask.reset(componentId);
		}

		struct EntityDesc
		{
			EntityId id;
			ComponentMask mask;
		};

		std::vector<EntityDesc> m_Entities;
		std::vector<EntityIndex> m_FreeEntities;
		std::vector<ComponentPool*> m_ComponentPools;

		Spatial2DGrid* m_Spatial2DGrid;

		void Cleanup();

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
	};


	// helpful Iterator to loop throught entities that have indicated components

	template<typename... ComponentTypes>
	class SceneView
	{
	public:
		SceneView(Scene* scene)
			: p_Scene(scene), m_All(false)
		{
			if (sizeof...(ComponentTypes) == 0)
			{
				m_All = true;
			}
			else
			{
				// unpac template parameters into an initializer list
				int componentsIds[] = { 0, GetComponentId<ComponentTypes>() ... };
				for (int i = 1; i < (sizeof...(ComponentTypes) + 1); i++)
				{
					m_ComponentMask.set(componentsIds[i]);
				}
			}
		}

		struct Iterator
		{
			Iterator(Scene* scene, EntityIndex index, ComponentMask mask, bool all)
				: p_Scene(scene), m_Index(index), m_Mask(mask), m_All(all)
			{
				// ... omitted for now?
			}

			bool isIndexValid()
			{
				return
					IsEntityValid(p_Scene->m_Entities[m_Index].id) &&
					// it has the correct component mask?
					(m_All || m_Mask == (m_Mask & p_Scene->m_Entities[m_Index].mask));
			}

			EntityId operator* () const
			{
				return p_Scene->m_Entities[m_Index].id;
			}
			bool operator==(const Iterator& other) const
			{
				return m_Index == other.m_Index || m_Index == p_Scene->m_Entities.size();
			}

			bool operator!=(const Iterator& other) const
			{
				return m_Index != other.m_Index && m_Index != p_Scene->m_Entities.size();
			}

			Iterator& operator++()
			{
				do
				{
					m_Index++;
				} while (m_Index < p_Scene->m_Entities.size() && !isIndexValid());
				return *this;
			}

			Scene* p_Scene;
			EntityIndex m_Index;
			ComponentMask m_Mask;
			bool m_All;
		};

		const Iterator begin() const
		{
			int firstIndex = 0;
			while (firstIndex < p_Scene->m_Entities.size() &&
				m_ComponentMask != (m_ComponentMask & p_Scene->m_Entities[firstIndex].mask) 
				|| !IsEntityValid(p_Scene->m_Entities[firstIndex].id))
			{
				firstIndex++;
			}
			return Iterator(p_Scene, firstIndex, m_ComponentMask, m_All);
		}

		// we are not supporting going backwards for now
		const Iterator end() const
		{
			return Iterator(p_Scene, EntityIndex(p_Scene->m_Entities.size()), m_ComponentMask, m_All);
		}

		Scene* p_Scene;
		ComponentMask m_ComponentMask;
		bool m_All;
	};
}

// utils
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