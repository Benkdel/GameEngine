#pragma once

#include <engine/dataStructures/SpatialHashGrid.h>
#include <engine/dataTypes.h>

#include <vector>


namespace Amba {

	class Scene;

	class Physics
	{
	public:

		Physics();
		~Physics();

		void SolveCollisions(Scene* scene);
		void ApplyMotion(Scene* scene, float dt);

		void inline ActivateGravity(bool status) { m_IsGravityOn = status; };
		bool inline IsGravityOn() { return m_IsGravityOn; };
		glm::vec3 inline GetGravity() { return m_Gravity; };

		inline void BindSpatialGrid(Spatial2DGrid* grid) { p_SpatialGrid = grid; };

		Spatial2DGrid* p_SpatialGrid;
	
	private:
		IntersectData CheckForCollision(EntityId id, Scene* scene);
		bool m_IsGravityOn;

		glm::vec3 m_Gravity;

		friend class Scene;
	};

}