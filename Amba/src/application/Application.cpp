#include "Application.h"
#include "engine/Window.h"

#include <imgui.h>

#include <engine/interface/inferface.h>

#include <engine/dataStructures/SpatialHashGrid.h>

namespace Amba {

	Application::Application()
		: AB_DeltaTime(0.0f), AB_TimeElapsed(0.0f), p_Window(NULL),
		m_ScrWidth(1280), m_ScrHeight(760), p_ActiveScene(NULL)
	{
		Amba::Log::Init();
		AB_TRACE("Application Created Succesfully");
	}

	Application::~Application()
	{
	}

	void Application::Init(const char* title, unsigned int width, unsigned int height)
	{
		p_Window = new Amba::Window("Resurge", width, height);
		m_ScrWidth = width;
		m_ScrHeight = height;

		// for now, init Imgui here
		// Setup Dear ImGui context
		p_Interface = new Interface();
		p_Interface->Setup(p_Window);

	}

	void Application::BindScene(Scene* scene)
	{
		AB_ASSERT(!scene==NULL, "Trying to bind a NULL scene");
		p_ActiveScene = scene;
		p_Interface->BindScene(scene);

		// initialize viewport data in Scene to be used with cameras and rendering
		p_ActiveScene->m_ViewPortData = ViewPortData({ m_ScrWidth, m_ScrHeight, false });
	}

	// method to be override by user
	void Application::OnUserCreate()
	{
	}

	// method to be override by user
	void Application::OnUserUpdate()
	{
	}


	// I should make any input (keyboard, mouse, joypad) virtual functions so user can override them if they want
	// another functionality
	bool Application::AB_keyPressed(unsigned int GLFW_KEY_CODE)
	{
		if (glfwGetKey(p_Window->getWindow(), GLFW_KEY_CODE) == GLFW_PRESS)
			return true;
		else
			return false;
	}

	bool Application::AB_keyReleased(unsigned int GLFW_KEY_CODE)
	{
		if (glfwGetKey(p_Window->getWindow(), GLFW_KEY_CODE) == GLFW_RELEASE)
			return true;
		else
			return false;
	}

	void Application::InterfaceHandler()
	{
		p_Interface->Run(AB_DeltaTime);
	}

	void Application::SetWindowShouldClose()
	{
		p_Window->SetWindowShouldClose();
	}

	void Application::Run()
	{
		double lastTime = 0.0f;

		// method used by user
		OnUserCreate();
	
		while (!p_Window->WindowShouldClose())
		{
			Renderer::Clear(p_Interface->m_Clear_color);

			// Compute delta time and store it as class member for user to use
			AB_TimeElapsed = glfwGetTime();
			AB_DeltaTime = AB_TimeElapsed - lastTime;
			lastTime = AB_TimeElapsed;

			// method used by user
			OnUserUpdate();
			
			// run interface - ImGui
			InterfaceHandler();

			ViewPortData vp = p_Window->OnUpdate();
			if (vp.m_HasChanged)
				p_ActiveScene->m_ViewPortData = vp;
			
			ProcessInput();
		}

		delete p_Window;
		p_Interface->Cleanup();
	}

	void Application::ProcessInput()
	{
		if (Amba::KeyBoard::KeyWentDown(GLFW_KEY_ESCAPE))
			SetWindowShouldClose();

		// lock/unlock mouse
		if (Amba::KeyBoard::KeyWentDown(GLFW_KEY_F10))
		{
			Amba::Mouse::ToggleMouseLock();
			p_Window->ToggleCursor();
		}
	}

}

