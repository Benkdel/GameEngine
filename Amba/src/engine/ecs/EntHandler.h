#pragma once

#include <engine/ecs/ComponentPool.h>
#include <engine/ecs/components.h>

#include <vector>
#include <bitset>

const int MAX_COMPONENTS = 32;
typedef std::bitset<MAX_COMPONENTS> ComponentMask;

typedef unsigned long long EntityId;
typedef unsigned int EntityIndex;
typedef unsigned int EntityVersion;

namespace Amba {

	class EntityHandler
	{
	public:

		EntityHandler() = default;
		~EntityHandler();

		// EntityID methods
		static EntityId CreateEntityId(EntityIndex index, EntityVersion version);
		static EntityIndex GetEntityIndex(EntityId id);
		static EntityVersion GetEntityVersion(EntityId id);
		static bool IsEntityValid(EntityId id);

		struct EntityDesc
		{
			EntityId id;
			ComponentMask mask;
		};

		std::vector<EntityDesc> m_Entities;
		std::vector<EntityIndex> m_FreeEntities;
		std::vector<ComponentPool*> m_ComponentPools;

	private: // templates

		template <typename T>
		bool EntHasComponent(EntityId id)
		{
			AB_ASSERT(IsEntityValid(id), "Invalid EntityId");
			int componentId = GetComponentId<T>();
			ComponentMask comp;
			comp.set(componentId);

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

		EntityId CreateEntity();
		EntityId CopyEntity(EntityId id);
		void DestroyEntity(EntityId id);

		friend class Scene;
	};

	// helpful Iterator to loop throught entities that have indicated components
	template<typename... ComponentTypes>
	class EntityGroup
	{
	public:
		EntityGroup(EntityHandler* entHandler)
			: p_EntityHandler(entHandler), m_All(false)
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
			Iterator(EntityHandler* entHandler, EntityIndex index, ComponentMask mask, bool all)
				: p_EntityHandler(entHandler), m_Index(index), m_Mask(mask), m_All(all)
			{
				// ... omitted for now?
			}

			bool isIndexValid()
			{
				return
					EntityHandler::IsEntityValid(p_EntityHandler->m_Entities[m_Index].id) &&
					// it has the correct component mask?
					(m_All || m_Mask == (m_Mask & p_EntityHandler->m_Entities[m_Index].mask));
			}

			EntityId operator* () const
			{
				return p_EntityHandler->m_Entities[m_Index].id;
			}
			bool operator==(const Iterator& other) const
			{
				return m_Index == other.m_Index || m_Index == p_EntityHandler->m_Entities.size();
			}

			bool operator!=(const Iterator& other) const
			{
				return m_Index != other.m_Index && m_Index != p_EntityHandler->m_Entities.size();
			}

			Iterator& operator++()
			{
				do
				{
					m_Index++;
				} while (m_Index < p_EntityHandler->m_Entities.size() && !isIndexValid());
				return *this;
			}

			EntityHandler* p_EntityHandler;
			EntityIndex m_Index;
			ComponentMask m_Mask;
			bool m_All;
		};

		const Iterator begin() const
		{
			int firstIndex = 0;

			while (firstIndex < p_EntityHandler->m_Entities.size() &&
				m_ComponentMask != (m_ComponentMask & p_EntityHandler->m_Entities[firstIndex].mask)
				|| !EntityHandler::IsEntityValid(p_EntityHandler->m_Entities[firstIndex].id))
			{
				firstIndex++;
			}
			return Iterator(p_EntityHandler, firstIndex, m_ComponentMask, m_All);
		}

		// we are not supporting going backwards for now
		const Iterator end() const
		{
			return Iterator(p_EntityHandler, EntityIndex(p_EntityHandler->m_Entities.size()), m_ComponentMask, m_All);
		}

		EntityHandler* p_EntityHandler;
		ComponentMask m_ComponentMask;
		bool m_All;
	};
}
