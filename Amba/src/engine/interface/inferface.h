#pragma once

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <imgui_internal.h>

#include <engine/renderer/Renderer.h>
#include <engine/Window.h>
#include <glm/glm.hpp>

#include <engine/ResourceManager.h>

#include <engine/interface/MousePicking.h>

#include <engine/interface/EditorCameraController.h>

namespace Amba {

	class Interface
	{
	public:
	
		Interface();

		void Setup(Window* window);

		void Run(double dt);
		void ProcessUserInput(Camera* camera, double dt);

		void BindScene(Scene* scene);

		void Cleanup();

		glm::vec4 m_Clear_color;

		inline void UpdateMouseD(double dx, double dy) { m_Yaw = dx; m_Pitch = dy; };

		// static and global variables
		static EntityId s_SelectedEntity;
		static EntityId s_EntUnderCursor;
		static EntityId s_SelectedCameraEntity;

		static std::string GetActiveEntity(EntityId ent);
		static std::string GetActiveEntityTag(EntityId ent, Scene* scene);

		enum STATUS
		{
			INNACTIVE,
			ACTIVE
		};

	private:
		Scene* p_CurrentScene;
		MousePicker m_MousePicker;

		Window* p_Window = nullptr;

		Camera* p_EditorCamera = nullptr;
		bool m_IsEditorCameraActive = true;

		Camera* p_ActiveCamera = nullptr;

		int m_Status = 0;

		double m_Yaw = 0.0f;
		double m_Pitch = 0.0f;

		unsigned int m_ScrWidth;
		unsigned int m_ScrHeight;

		bool m_firstRunFlag = true;
	};

}
