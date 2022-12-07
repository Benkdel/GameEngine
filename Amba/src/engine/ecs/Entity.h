#pragma once

#include <engine/ecs/Scene.h>


namespace Amba {
		
	class Entity
	{
	public:
		
		Entity() = default;
		Entity(Scene* scene, std::string tag = "undefined");
		Entity(Scene* scene, EntityId id, std::string tag = "undefined");

		~Entity();

		Entity CopyEntity(std::string tag = "undefined");
		void Destroy();

		inline EntityId GetEntId() { return m_EntId; };
		inline Scene* GetScene() { return p_Scene; };

		bool InitCollider();

		// tags
		void AddTag(const std::string tag);
		std::string GetTag();
		void DeleteTag();
		void ModifyTag(std::string newTag);

		// how????
		Entity* p_Child = nullptr;


	public: // templates

		template<typename T>
		T* AddComponent()
		{
			AB_ASSERT(!HasComponent<T>(), "Entity already has component");
			return p_Scene->AddComponent<T>(m_EntId);
		}

		template<typename T>
		T* GetComponent()
		{
			AB_ASSERT(HasComponent<T>(), "Entity does not have component");
			return p_Scene->GetComponent<T>(m_EntId);
		}

		template<typename T>
		void RemoveComponent()
		{
			AB_ASSERT(HasComponent<T>(), "Entity does not have component");
			p_Scene->RemoveComponent(m_EntId);
		}

		template<typename T>
		bool HasComponent()
		{
			AB_ASSERT(IsEntityValid(m_EntId), "Entity is invalid");
			return p_Scene->EntHasComponent<T>(m_EntId);
		}
		
		operator bool() const { return IsEntityValid(m_EntId); };

	private:
		EntityId m_EntId = -1;
		Scene* p_Scene;
	};

}


