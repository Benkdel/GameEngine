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
				// add to this whenever a new component is added
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

	// spatial grid methods
	void Scene::AssignEntity(EntityId id)
	{
		AB_WARN("DONT USE THIS VERSION OF ASSIGN ENTITY FOR NOW! USE THE ONE THAT TAKES POSITION ALSO");
		m_Spatial2DGrid.GetCell(this->GetComponent<TransformComponent>(id)->m_Position).entities.push_back(id);
	}

	void Scene::AssignEntity(EntityId id, glm::vec3 position)
	{
		TransformComponent* tsr = GetComponent<TransformComponent>(id);
		
		int oldCellIndex = tsr->m_CurrentCell;
		
		int cell_x = position.x / m_Spatial2DGrid.m_CellSize;
		int cell_z = position.z / m_Spatial2DGrid.m_CellSize;
		int newCellIndex = cell_z * m_Spatial2DGrid.m_CellSize + cell_x;

		if (oldCellIndex == newCellIndex)
			return;

		if (oldCellIndex >= 0)
		{
			// delete entity from old cell vector (bookkeeping)
			int idx = tsr->m_IndexInCell;
			m_Spatial2DGrid.m_Cells[oldCellIndex].entities.erase(m_Spatial2DGrid.m_Cells[oldCellIndex].entities.begin() + idx);
		}

		tsr->m_CurrentCell = newCellIndex;
		tsr->m_IndexInCell = (int)m_Spatial2DGrid.GetCell(position).entities.size();

		m_Spatial2DGrid.GetCell(position).entities.push_back(id);
	
		tsr = nullptr;
	}

	void Scene::FindNearEntities(EntityId id, glm::vec3 position)
	{
		Cell& cell = m_Spatial2DGrid.GetCell(position);

		std::vector<Cell> cellsToCheck;

		// check if entity is completly inside the cell
		// for now im using position plus constant but I should use box collider or sphere collider instead
		// center plus radius or collision box, maight be easier

		cellsToCheck.push_back(cell);

		// very simple for now just to do tests, improve this later

		float tempSize = 1.0f;
		bool BL = position.x - tempSize > cell.bottomLeft.x;
		bool BR = position.x + tempSize < cell.bottomRight.x;
		bool TL = position.z - tempSize > cell.bottomLeft.z;
		bool TR = position.z + tempSize < cell.topLeft.z;
		
		if (!BL)
			cellsToCheck.push_back(m_Spatial2DGrid.GetCell(position - glm::vec3(tempSize, 0.0f, 0.0f)));
		if (!BR)
			cellsToCheck.push_back(m_Spatial2DGrid.GetCell(position + glm::vec3(tempSize, 0.0f, 0.0f)));
		if (!TL)
			cellsToCheck.push_back(m_Spatial2DGrid.GetCell(position - glm::vec3(0.0f, 0.0f, tempSize)));

		// find all entities in cells and change their color (implement this option in shader)

		for (auto cell : cellsToCheck)
		{
			for (auto ent : cell.entities)
			{
				if (ent != id)
					this->GetComponent<TransformComponent>(ent)->m_Size = 0.2f;
			}
		}
	}

}
