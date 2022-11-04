#pragma once

#include <engine/Window.h>
#include <engine/Log.h>
#include <engine/core.h>
#include <engine/io/Keyboard.h>
#include <engine/io/Mouse.h>
#include <engine/io/Camera.h>
#include <engine/renderer/Renderer.h>

#include <engine/interface/inferface.h>
#include <engine/renderer/dataTypes.h>

class Scene;

namespace Amba {

class Application
{
	public:
		Application();
		~Application();

		void Run();

		void Init(const char* title = "Game", unsigned int width = 1280, unsigned int height = 760);
		
		void BindScene(Scene* scene);

		bool AB_keyPressed(unsigned int GLFW_KEY_CODE);
		bool AB_keyReleased(unsigned int GLFW_KEY_CODE);

		void InterfaceHandler();

		// methods to be overrided by user
		virtual void OnUserCreate();
		virtual void OnUserUpdate();

		void SetWindowShouldClose();

		// cameras
		unsigned int AB_ActiveCamera;
		Amba::Camera AB_Cameras[5];

		Scene* p_ActiveScene;

	private:
		Amba::Window* p_Window;
		Amba::Interface* p_Interface;

		void ProcessInput();


	protected:
		double AB_TimeElapsed;
		double AB_DeltaTime;


		unsigned int m_ScrWidth;
		unsigned int m_ScrHeight;

	};
}

