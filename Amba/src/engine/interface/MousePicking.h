#pragma once

#include <engine/dataTypes.h>
#include <engine/dataStructures/SpatialHashGrid.h>
#include <engine/ecs/Scene.h>

#include <engine/io/Camera.h>

#include <engine/io/Mouse.h>

namespace Amba {


	class MousePicker 
	{
	public:

		MousePicker();

		void SetUp(unsigned int scr_Width, unsigned int scr_Height);

		void UpdateMousePos(Camera* camera);

		inline void UpdateScene(Scene* scene) { p_Scene = scene; };

		EntityId SelectEntity(Camera* camera);

		inline glm::vec3 GetMouseRay() { return m_MouseRay; };

	private:
		glm::vec3 NormalizeDeviceSpace(glm::vec3 mousePosition);
		glm::vec4 ClipSpace(glm::vec3 mousePos);
		glm::vec4 EyeSpace(glm::vec4 mousePos, Camera* camera);
		glm::vec3 WorldSpace(glm::vec4 mousePos, Camera* camera);

		glm::vec3 m_MouseRay;

		unsigned int m_ScrWidth;
		unsigned int m_ScrHeight;

		Scene* p_Scene;
	};

}
