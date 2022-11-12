#pragma once

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include <engine/Window.h>
#include <engine/renderer/Renderer.h>
#include <glm/glm.hpp>

#include <engine/ResourceManager.h>

#include <engine/interface/MousePicking.h>

namespace Amba {

	class Interface
	{
	public:
	
		Interface();

		void Setup(Window* window);

		void Run(Camera& camera);

		void BindScene(Scene* scene);

		void Cleanup();

		glm::vec4 m_Clear_color;

	
	private:
		Scene* p_CurrentScene;
		MousePicker m_MousePicker;
	};

}
