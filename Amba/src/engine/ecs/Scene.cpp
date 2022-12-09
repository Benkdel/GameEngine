#include "Scene.h"

int s_CompotentCounter = 0;

#include <engine/io/Mouse.h>
#include <engine/ecs/Entity.h>
#include <engine/ResourceManager.h>
#include <engine/renderer/Renderer.h>

namespace Amba {

	// entity handlers
	EntityId CreateEntityId(EntityIndex index, EntityVersion version)
	{
		// Shifts the index up 32, and puts the version in the bottom
		return ((EntityId)index << 32) | ((EntityId)version);
	}

	EntityIndex GetEntityIndex(EntityId id)
	{
		// Shifts down 32 so we lose the version part and get our index
		return id >> 32;
	}

	EntityVersion GetEntityVersion(EntityId id)
	{
		// Cast to a 32 bit int to get our version number (loosing the top 32 bits)
		return (EntityVersion)id;
	}

	bool IsEntityValid(EntityId id)
	{
		// check if the index is the invalid index
		return (id >> 32) != EntityIndex(-1);
	}


	/* ===================================================================================== */

	void Scene::Update(float dt)
	{
		Amba::Renderer::BeginScene(GetComponent<CameraComponent>(m_ActiveCamera)->GetCamera(), 
			this);

		// things to update both in interface and play mode
		GetComponent<CameraComponent>(m_ActiveCamera)->GetCamera()->NewUpdateCameraPos(
			GetComponent<TransformComponent>(m_ActiveCamera)->GetPosition(), dt);

		// assign entities to spatial grid in scene - exclude plane colliders for now
		for (EntityId ent : SceneView<PhysicsComponent>(this))
			AssignEntity(ent);

		// render - for now im using simple shader as default, which is being loaded in game app
		// TODO: change so it loads a base shader at begining and use it as defaul, or always ask for shaders in 
		// entities that have render component (also TODO)
		Amba::Renderer::DrawActiveScene(ResManager::GetShader("simpleShader"),
			GetComponent<CameraComponent>(m_ActiveCamera)->GetCamera()->GetPerspective(m_ViewPortData));

		// If game is not paused
		if (!Amba::Mouse::isMouseLocked())
		{
			// Apply physics
			Amba::Physics::SolveCollisions(this);
			Amba::Physics::ApplyMotion(this, dt);
		}

	}

	Scene::Scene()
	{
		m_Spatial2DGrid = new Spatial2DGrid(false);

		// create an entity handler for the editor (default) camera
		EntityId cam = CreateEntity();
		AddComponent<TransformComponent>(cam);
		AddComponent<CameraComponent>(cam);
		AddComponent<TagComponent>(cam);
		GetComponent<TagComponent>(cam)->m_Tag = EDITOR_CAMERA_TAG;
		SetActiveCamera(cam);
	}

	EntityId Scene::GetEntityByTag(const std::string& tag)
	{

		for (EntityId ent : SceneView<TagComponent>(this))
		{
			if (GetComponent<TagComponent>(ent)->m_Tag == tag)
				return ent;
		}
		return -1;
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
				MeshComponent*			mesh		= dynamic_cast<MeshComponent*>((Components*)component);
				TransformComponent*		tsr			= dynamic_cast<TransformComponent*>((Components*)component);
				SphereCollider*			spColl		= dynamic_cast<SphereCollider*>((Components*)component);
				AABCollider*			aabColl		= dynamic_cast<AABCollider*>((Components*)component);
				PlaneCollider*			planeColl	= dynamic_cast<PlaneCollider*>((Components*)component);
				PhysicsComponent*		physics		= dynamic_cast<PhysicsComponent*>((Components*)component);
				CameraComponent*		camera		= dynamic_cast<CameraComponent*>((Components*)component);
				ControllerComponent*	controller	= dynamic_cast<ControllerComponent*>((Components*)component);
				TagComponent*			tag			= dynamic_cast<TagComponent*>((Components*)component);
				AudioComponent*			audio		= dynamic_cast<AudioComponent*>((Components*)component);

				if (mesh != NULL)			    *AddComponent<MeshComponent>(target) = *GetComponent<MeshComponent>(source);
				else if (tsr != NULL)			*AddComponent<TransformComponent>(target) = *GetComponent<TransformComponent>(source);
				else if (spColl != NULL)		*AddComponent<SphereCollider>(target) = *GetComponent<SphereCollider>(source);
				else if (aabColl != NULL)		*AddComponent<AABCollider>(target) = *GetComponent<AABCollider>(source);
				else if (planeColl != NULL)		*AddComponent<PlaneCollider>(target) = *GetComponent<PlaneCollider>(source);
				else if (physics != NULL)		*AddComponent<PhysicsComponent>(target) = *GetComponent<PhysicsComponent>(source);
				else if (camera != NULL)		*AddComponent<CameraComponent>(target) = *GetComponent<CameraComponent>(source);
				else if (tag != NULL)			*AddComponent<TagComponent>(target) = *GetComponent<TagComponent>(source);
				else if (controller != NULL)	*AddComponent<ControllerComponent>(target) = *GetComponent<ControllerComponent>(source);
				else if (audio != NULL)			*AddComponent<AudioComponent>(target)		= *GetComponent<AudioComponent>(source);
				else							AB_WARN("Component not recognized!");
			}
		}
		return target;
	}

	void Scene::DestroyEntity(EntityId id)
	{
		// make entity invalid (-1) and save old id in free entities so we can use it later
		EntityId newId = CreateEntityId((EntityIndex)-1, GetEntityVersion(id) + 1);
		EntityIndex oldIdx = GetEntityIndex(id);

		m_Entities[oldIdx].id = newId;
		m_Entities[oldIdx].mask.reset();
		m_FreeEntities.push_back(oldIdx);
	}
	
	
	void Scene::AddCameraObject(Entity* entity, bool primary)
	{
		std::vector<EntityId>::iterator it = std::find(m_AvailableCameras.begin(),
			m_AvailableCameras.end(), entity->GetEntId());

		AB_ASSERT(it == m_AvailableCameras.end(), "Camera object has already been added");

		m_AvailableCameras.push_back(entity->GetEntId());
		if (primary)
		{
			m_ActiveCamera = entity->GetEntId();
		}
	}

	void Scene::SetActiveCamera(Entity* entity)
	{
		std::vector<EntityId>::iterator it = std::find(m_AvailableCameras.begin(),
			m_AvailableCameras.end(), entity->GetEntId());

		AB_ASSERT(!(it == m_AvailableCameras.end()), "Camera object not found");

		m_ActiveCamera = entity->GetEntId();
	}

	void Scene::SetActiveCamera(EntityId id)
	{
		AB_ASSERT(EntHasComponent<CameraComponent>(id), "Invalid Camera Object");
		m_ActiveCamera = id;
	}

	Camera* Scene::GetActiveCamera()
	{ 
		return GetComponent<CameraComponent>(m_ActiveCamera)->GetCamera();
	};

	void Scene::DeleteCamera(Entity* entity)
	{
		std::vector<EntityId>::iterator it = std::find(m_AvailableCameras.begin(),
			m_AvailableCameras.end(), entity->GetEntId());

		AB_ASSERT(!(it == m_AvailableCameras.end()), "Camera object not found");

		m_AvailableCameras.erase(it);
	}


	// these methods for spatial grid should not be here
	void Scene::AssignEntity(EntityId id)
	{
		if (!IsEntityValid(id))
			return;

		if (EntHasComponent<CameraComponent>(id))
			return;

		TransformComponent* tsr = GetComponent<TransformComponent>(id);
		GridCell gridCell = m_Spatial2DGrid->GetGridCell(tsr->GetPosition());
		
		int oldCellIndex		= tsr->m_CurrentCell;
		int oldIndexInCell		= tsr->m_IndexInCell;
		int newCellIndex		= -1;
		
		if (gridCell.IsCellValid())
		{
			Cell& cell			= gridCell.GetCell();
			newCellIndex		= cell.GetCellIdx();
			tsr->m_CurrentCell	= newCellIndex;
		
			// no change == skip
			if (oldCellIndex == newCellIndex)
				return;

			cell.entities.push_back(id);
			tsr->m_IndexInCell = m_Spatial2DGrid->m_Cells[newCellIndex].entities.size() - 1;
		}

		// delete entity from old cell vector (bookkeeping)
		if (oldCellIndex >= 0)
		{
			Cell& oldCell = m_Spatial2DGrid->m_Cells[oldCellIndex];
			oldCell.entities.
					erase(oldCell.entities.begin() + oldIndexInCell);

			// for now, move all entities to the right of ent being assigned one position to the left (vector size is reduced)
			for (size_t i = oldIndexInCell; i < oldCell.entities.size(); i++)
			{
				GetComponent<TransformComponent>(oldCell.entities[i])->m_IndexInCell -= 1;
			}
		}

		// if entity is out of world map boundaries, destroy it.
		if (m_Spatial2DGrid->IsOutsideBoundaries(tsr->GetPosition()))
		{
			DestroyEntity(id);
			return;
		}

		tsr = nullptr;
	}

	std::vector<Cell> Scene::GetNearbyCells(glm::vec3 position)
	{
		GridCell gridCell = m_Spatial2DGrid->GetGridCell(position);
		std::vector<Cell> cellsToCheck;

		if (!gridCell.IsCellValid())
			return cellsToCheck;

		Cell& cell = gridCell.GetCell();

		// check if entity is completly inside the cell
		// WARNING: for now this only works for entities smaller than cells

		cellsToCheck.push_back(cell);
		
		return cellsToCheck;
	}


	void Scene::Cleanup()
	{
		// todo
	}

}
