#include "Entity.h"

#include <engine/ResourceManager.h>

namespace Amba {
	

	Entity::Entity(Scene* scene, EntityId id)
	{
		p_Scene = scene;

		if (scene->p_EntHandler->IsEntityValid(id))
		{
			m_EntId = id;
			return;
		}
		else
			m_EntId = scene->CreateEntity();

		// every entity should have a transform
		AddComponent<TransformComponent>();
	}

	Entity::~Entity()
	{
		// Im not destroying this because when class goes out of scope I still have
		// the EntityId types in the scene class
		AB_INFO("Entity {0} destroid ", m_EntId);
		// Destroy();
	}

	Entity Entity::CopyEntity()
	{
		EntityId newEnt = p_Scene->CopyEntity(m_EntId);
		return Entity(p_Scene, newEnt);
	}

	void Entity::Destroy()
	{
		p_Scene->DestroyEntity(m_EntId);
		p_Scene = nullptr;
		m_EntId = -1;
	}

	void Entity::AddChildren(Entity* ent)
	{
		m_Entities.push_back(ent);
	}

	bool Entity::InitCollider()
	{
		int componentID = GetColliderTypeIndex(m_EntId, p_Scene);

		if (componentID < 0)
			return false;

		ComponentMask entMask = p_Scene->p_EntHandler->m_Entities[p_Scene->p_EntHandler->GetEntityIndex(m_EntId)].mask;

		if (!entMask.test(componentID))
			return false;

		ColliderComponent* collider = p_Scene->GetComponentWithId<ColliderComponent>(m_EntId, componentID);

		MeshComponent* mesh = GetComponent<MeshComponent>();
		TransformComponent* tsr = GetComponent<TransformComponent>();

		collider->InitCollider(mesh, tsr);

		collider = nullptr;
		mesh = nullptr;
		tsr = nullptr;

		return true;
	}
}
