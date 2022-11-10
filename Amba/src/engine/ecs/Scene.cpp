#include "Scene.h"


namespace Amba {

	Scene::Scene() 
	{
		m_Spatial2DGrid = Spatial2DGrid(true);
	}

	void Scene::ApplyPhysics()
	{
		for (EntityId ent : SceneView<TransformComponent, MeshComponent>(this))
		{
			glm::vec3& position = GetComponent<TransformComponent>(ent)->m_Position;
			glm::vec3& velocity = GetComponent<TransformComponent>(ent)->m_Velocity;

			position += velocity * glm::vec3(0.01f);

			if (position.x <= 0.0f || position.x >= 5.0f)
				velocity.x *= -1.0f;
			if (position.y <= 0.0f || position.y >= 5.0f)
				velocity.y *= -1.0f;
			if (position.z <= -5.0f || position.z >= 5.0f)
				velocity.z *= -1.0f;
			
			// assign entity to cell in 2D spatial grid
			AssignEntity(ent, position);
		}
	}

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

	void Scene::AssignEntity(EntityId id)
	{
		Cell& cell = m_Spatial2DGrid.GetCell(this->GetComponent<TransformComponent>(id)->m_Position);
		cell.entities.push_back(id);
	}

	void Scene::AssignEntity(EntityId id, glm::vec3 position)
	{
		Cell& cell = m_Spatial2DGrid.GetCell(position);
		cell.entities.push_back(id);
	}

}
