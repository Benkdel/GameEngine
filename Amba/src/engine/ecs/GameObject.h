#pragma once

#include <engine/ecs/Entcs.h>
#include <engine/dataTypes.h>

#include <vector>


class Scene;

namespace Amba {

	class GameObject
	{
	public:

		GameObject() = default;


		
		

		std::vector<Entity> m_Entities;

	private:
		Scene* p_Scene;
		

	};

}