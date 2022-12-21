#include "EntHandler.h"
#include <engine/core.h>
#include <engine/physics/Collision.h>

namespace Amba {

	EntityHandler::~EntityHandler()
	{
#ifdef AB_ENABLE_ASSERTS
		AB_WARN("EntityHandler destructor called");
#endif
	}

	EntityId EntityHandler::CreateEntityId(EntityIndex index, EntityVersion version)
	{
		// Shifts the index up 32, and puts the version in the bottom
		return ((EntityId)index << 32) | ((EntityId)version);
	}

	EntityIndex EntityHandler::GetEntityIndex(EntityId id)
	{
		// Shifts down 32 so we lose the version part and get our index
		return id >> 32;
	}

	EntityVersion EntityHandler::GetEntityVersion(EntityId id)
	{
		// Cast to a 32 bit int to get our version number (loosing the top 32 bits)
		return (EntityVersion)id;
	}

	bool EntityHandler::IsEntityValid(EntityId id)
	{
		// check if the index is the invalid index
		return (id >> 32) != EntityIndex(-1);
	}

	EntityId EntityHandler::CreateEntity()
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

	EntityId EntityHandler::CopyEntity(EntityId source)
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
				SphereCollider* spColl = dynamic_cast<SphereCollider*>((Components*)component);
				AABCollider* aabColl = dynamic_cast<AABCollider*>((Components*)component);
				PlaneCollider* planeColl = dynamic_cast<PlaneCollider*>((Components*)component);
				PhysicsComponent* physics = dynamic_cast<PhysicsComponent*>((Components*)component);
				CameraComponent* camera = dynamic_cast<CameraComponent*>((Components*)component);
				ControllerComponent* controller = dynamic_cast<ControllerComponent*>((Components*)component);
				TagComponent* tag = dynamic_cast<TagComponent*>((Components*)component);
				AudioComponent* audio = dynamic_cast<AudioComponent*>((Components*)component);

				if (mesh != NULL)			    *AddComponent<MeshComponent>(target) = *GetComponent<MeshComponent>(source);
				else if (tsr != NULL)			*AddComponent<TransformComponent>(target) = *GetComponent<TransformComponent>(source);
				else if (spColl != NULL)		*AddComponent<SphereCollider>(target) = *GetComponent<SphereCollider>(source);
				else if (aabColl != NULL)		*AddComponent<AABCollider>(target) = *GetComponent<AABCollider>(source);
				else if (planeColl != NULL)		*AddComponent<PlaneCollider>(target) = *GetComponent<PlaneCollider>(source);
				else if (physics != NULL)		*AddComponent<PhysicsComponent>(target) = *GetComponent<PhysicsComponent>(source);
				else if (camera != NULL)		*AddComponent<CameraComponent>(target) = *GetComponent<CameraComponent>(source);
				else if (tag != NULL)			*AddComponent<TagComponent>(target) = *GetComponent<TagComponent>(source);
				else if (controller != NULL)	*AddComponent<ControllerComponent>(target) = *GetComponent<ControllerComponent>(source);
				else if (audio != NULL)			*AddComponent<AudioComponent>(target) = *GetComponent<AudioComponent>(source);
				else							AB_WARN("Component not recognized!");
			}
		}
		return target;
	}

	void EntityHandler::DestroyEntity(EntityId id)
	{
		// make entity invalid (-1) and save old id in free entities so we can use it later
		EntityId newId = CreateEntityId((EntityIndex)-1, GetEntityVersion(id) + 1);
		EntityIndex oldIdx = GetEntityIndex(id);

		m_Entities[oldIdx].id = newId;
		m_Entities[oldIdx].mask.reset();
		m_FreeEntities.push_back(oldIdx);
	}

}