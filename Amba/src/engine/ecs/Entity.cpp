#include "Entity.h"

#include <engine/ResourceManager.h>

namespace Amba {

	Entity::Entity(Scene* scene, std::string tag)
	{
		p_Scene = scene;
		m_EntId = scene->CreateEntity();

		if (std::strcmp(tag.c_str(), "undefined") != 0)
			AddTag(tag);

		// every entity should have a transform
		AddComponent<TransformComponent>();
	}

	// check this - method only to be used by copy entity
	Entity::Entity(Scene* scene, EntityId id, std::string tag)
	{
		p_Scene = scene;
		m_EntId = id;

		if (std::strcmp(tag.c_str(), "undefined") != 0)
			AddTag(tag);
	}

	Entity::~Entity()
	{
		// Im not destroying this because when class goes out of scope I still have
		// the EntityId types in the scene class

		// Destroy();
	}

	Entity Entity::CopyEntity(std::string tag)
	{
		EntityId newEnt = p_Scene->CopyEntity(m_EntId);
		return Entity(p_Scene, newEnt, tag);
	}

	void Entity::Destroy()
	{
		p_Scene->DestroyEntity(m_EntId);
		p_Scene->DeleteTag(m_EntId);
		p_Scene = nullptr;
		m_EntId = -1;
	}


	bool Entity::InitCollider()
	{
		int componentID = GetColliderTypeIndex(m_EntId, p_Scene);

		if (componentID < 0)
			return false;

		ComponentMask entMask = p_Scene->m_Entities[GetEntityIndex(m_EntId)].mask;

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

	void Entity::AddTag(const std::string tag)
	{
		p_Scene->AddTag(m_EntId, tag);
	}

	std::string Entity::GetTag()
	{
		return p_Scene->GetTag(m_EntId);
	}

	void Entity::DeleteTag()
	{
		p_Scene->DeleteTag(m_EntId);
	}

	void Entity::ModifyTag(std::string newTag)
	{
		p_Scene->ModifyTag(m_EntId, newTag);
	}

}