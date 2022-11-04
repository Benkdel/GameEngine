#include "Scene.h"

#include <application/Application.h>


namespace Amba {

	Scene::Scene() {}

	EntityId Scene::CreateEntity()
	{
		if (!m_FreeEntities.empty())
		{
			EntityIndex newIndex = m_FreeEntities.back();
			m_FreeEntities.pop_back();
			EntityId newId = CreateEntityId(newIndex, GetEntityVersion(m_Entities[newIndex].id));
			m_Entities[newIndex].id = newId;
			return m_Entities[newIndex].id;
		}

		m_Entities.push_back({ CreateEntityId((EntityIndex)m_Entities.size(), 0), ComponentMask() });
		return m_Entities.back().id;
	}

	EntityId Scene::CopyEntity(EntityId source)
	{
		EntityId target = CreateEntity();

		// copy all components to new entity
		ComponentMask mask = m_Entities[GetEntityIndex(source)].mask;
		
		for (int bit = 0; bit < mask.size(); bit++)
		{
			if (mask.test(bit))
			{
				// means this component is active
				void* component = m_ComponentPools[bit]->get(GetEntityIndex(source));
			
				// maybe I can make a function to return which component I need to cast it to
				MeshComponent* mesh = dynamic_cast<MeshComponent*>((Components*)component);
				TransformComponent* tsr = dynamic_cast<TransformComponent*>((Components*)component);
				CollisionComponent* col = dynamic_cast<CollisionComponent*>((Components*)component);
				AudioComponent* audio = dynamic_cast<AudioComponent*>((Components*)component);

				if (mesh != NULL)			*AddComponent<MeshComponent>(target)		= *GetComponent<MeshComponent>(source);
				else if (tsr != NULL)		*AddComponent<TransformComponent>(target)	= *GetComponent<TransformComponent>(source);
				else if (col != NULL)		*AddComponent<CollisionComponent>(target)	= *GetComponent<CollisionComponent>(source);
				else if (audio != NULL)		*AddComponent<AudioComponent>(target)		= *GetComponent<AudioComponent>(source);
				else						AB_WARN("Component not recognized!");
			}
		}
		return target;
	}

	void Scene::DestroyEntity(EntityId id)
	{
		EntityId newId = CreateEntityId((EntityIndex)-1, GetEntityVersion(id) + 1);
		EntityIndex oldIdx = GetEntityIndex(id);

		m_Entities[oldIdx].id = newId;
		m_Entities[oldIdx].mask.reset();
		m_FreeEntities.push_back(oldIdx);
	}
}
