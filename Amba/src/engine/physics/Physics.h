#pragma once

#include <engine/dataStructures/SpatialHashGrid.h>
#include <engine/dataTypes.h>

#include <vector>


namespace Amba {

	class Scene;

	class Physics
	{
	public:

		static void SolveCollisions(Scene* scene);
		static void ApplyMotion(Scene* scene, float dt);

		static float m_Gravity;

	private:
		static IntersectData CheckForCollision(EntityId id, Scene* scene);


	};

}