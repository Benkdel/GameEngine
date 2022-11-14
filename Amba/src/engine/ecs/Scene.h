#pragma once

#include <engine/ecs/ComponentPool.h>
#include "components.h"

#include <engine/dataStructures/SpatialHashGrid.h>
#include <engine/dataTypes.h>

#include <bitset>
#include <vector>

const int MAX_COMPONENTS = 32;
typedef std::bitset<MAX_COMPONENTS> ComponentMask;

namespace Amba {

	// entity handlers
	EntityId CreateEntityId(EntityIndex index, EntityVersion version);
	EntityIndex GetEntityIndex(EntityId id);
	EntityVersion GetEntityVersion(EntityId id);
	bool IsEntityValid(EntityId id);

	class Scene 
	{

	public:
		Scene();

		void ApplyPhysics();

	// spatial grid methods
	public:
		void AssignEntity(EntityId id);
		void AssignEntity(EntityId id, glm::vec3 position);

		std::vector<Cell> GetNearbyCells(glm::vec3 position);

		void FindNearEntities(EntityId id);

		void CheckForCollision(EntityId id);


	// entity handling section
	public:
		EntityId CreateEntity();

		EntityId CopyEntity(EntityId id);

		void DestroyEntity(EntityId id);

		template<typename T>
		T* AddComponent(EntityId id)
		{
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

		template<typename T>
		T* GetComponent(EntityId id)
		{
			int componentId = GetComponentId<T>();
			if (!m_Entities[GetEntityIndex(id)].mask.test(componentId))
				return nullptr;
		
			T* component = static_cast<T*>(m_ComponentPools[componentId]->get(GetEntityIndex(id)));
			return component;
		}

		template<typename T>
		void RemoveComponent(EntityId id)
		{
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
	
		Spatial2DGrid m_Spatial2DGrid;
	
		void Cleanup();

	private:


	};

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