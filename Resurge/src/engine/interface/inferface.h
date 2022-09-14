#pragma once

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include <engine/Window.h>
#include <engine/renderer/Renderer.h>
#include <glm/glm.hpp>

#include <engine/ResourceManager.h>

namespace Amba {

	class Interface
	{
	public:
	
		Interface();

		void Setup(Window* window);

		void Run();

		void Cleanup();

		glm::vec4 m_Clear_color;

	private:
		Window* m_Window;

		ImGuiViewport* m_MainViewport;

	};

}
