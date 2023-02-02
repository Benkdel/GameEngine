#include "Scene.h"

int s_CompotentCounter = 0;

#include <engine/io/Mouse.h>
#include <engine/scene/Entity.h>
#include <engine/ResourceManager.h>
#include <engine/renderer/Renderer.h>


namespace Amba {

	void Scene::Update(float dt)
	{
		Amba::Renderer::BeginScene(GetComponent<CameraComponent>(m_ActiveCamera)->GetCamera(), 
			this);

		// things to update both in interface and play mode
		GetComponent<CameraComponent>(m_ActiveCamera)->GetCamera()->NewUpdateCameraPos(
			GetComponent<TransformComponent>(m_ActiveCamera)->GetPosition(), dt);

		// assign entities to spatial grid in scene - exclude plane colliders for now
		
		if (p_PhysicsEngine != nullptr)
		{
			for (EntityId ent : EntityGroup<PhysicsComponent>(p_EntHandler))
				p_PhysicsEngine->p_SpatialGrid->AssignEntity(ent, this);

			// If game is not paused
			if (!Amba::Mouse::isMouseLocked())
			{
				// Apply physics
				p_PhysicsEngine->SolveCollisions(this);
				p_PhysicsEngine->ApplyMotion(this, dt);
			}

		}

		// check entities with fathers and update as offset?
		// what can I do do with scale? rotation? this is just position for now
		for (auto& ent : ResManager::rm_Entities)
		{
			Entity* current = ent.second;

			if (current->p_Father != nullptr)
			{
				glm::vec3 fatherPosition = current->p_Father->GetComponent<TransformComponent>()->GetPosition();

				current->GetComponent<TransformComponent>()->UpdatePosition(
					current->GetComponent<TransformComponent>()->GetPosition() +
					fatherPosition);
			}

		}

		// render - for now im using simple shader as default, which is being loaded in game app
		// TODO: change so it loads a base shader at begining and use it as defaul, or always ask for shaders in 
		// entities that have render component (also TODO)
		Amba::Renderer::DrawActiveScene(ResManager::GetShader("simpleShader"),
			GetComponent<CameraComponent>(m_ActiveCamera)->GetCamera()->GetPerspective(m_ViewPortData));
	}

	Scene::Scene()
	{
		// create an entity handler for the editor (default) camera
		p_EntHandler = new EntityHandler();
		EntityId cam = CreateEntity();
		AddComponent<TransformComponent>(cam);
		AddComponent<CameraComponent>(cam);
		AddComponent<TagComponent>(cam);
		GetComponent<TagComponent>(cam)->m_Tag = EDITOR_CAMERA_TAG;
		SetActiveCamera(cam);

		// init physics engine, for now
		p_PhysicsEngine = new Physics();
	}

	EntityId Scene::GetEntityByTag(const std::string& tag)
	{
		for (EntityId ent : EntityGroup<TagComponent>(p_EntHandler))
		{
			if (GetComponent<TagComponent>(ent)->m_Tag == tag)
				return ent;
		}
		return -1;
	}

	EntityId Scene::CreateEntity()
	{
		return p_EntHandler->CreateEntity();
	}

	EntityId Scene::CopyEntity(EntityId source)
	{
		return p_EntHandler->CopyEntity(source);
	}

	void Scene::DestroyEntity(EntityId id)
	{
		p_EntHandler->DestroyEntity(id);
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

	void Scene::Cleanup()
	{
		// todo
	}

}
