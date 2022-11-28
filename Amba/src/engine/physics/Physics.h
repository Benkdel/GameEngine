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

		static void inline ActivateGravity(bool status) { m_IsGravityOn = status; };
		static bool inline IsGravityOn() { return m_IsGravityOn; };
		static glm::vec3 inline GetGravity() { return m_Gravity; };

	private:
		static IntersectData CheckForCollision(EntityId id, Scene* scene);
		static bool m_IsGravityOn;

		static glm::vec3 m_Gravity;
	};

}