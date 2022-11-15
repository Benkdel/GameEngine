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

		// set active camera to 0 - default camera
		AB_ActiveCamera = 0;
	}

	void Application::BindScene(Scene* scene)
	{
		p_ActiveScene = scene;
		p_Interface->BindScene(scene);
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
		p_Interface->Run(AB_Cameras[AB_ActiveCamera]);
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
		
			AB_Perspective = glm::perspective(glm::radians(AB_Cameras[AB_ActiveCamera].GetZoom()), (float)m_ScrWidth / (float)m_ScrHeight, NEAR_PLANE, FAR_PLANE);

			// method used by user
			OnUserUpdate();
			
			// run interface - ImGui
			InterfaceHandler();

			p_Window->OnUpdate();

			ProcessInput();

		}

		delete p_Window;
		p_Interface->Cleanup();
	}


	void Application::ProcessInput()
	{
		if (Amba::KeyBoard::KeyWentDown(GLFW_KEY_ESCAPE))
			SetWindowShouldClose();

		// Camera movements
		if (Amba::KeyBoard::Key(GLFW_KEY_W))
		{
			AB_Cameras[AB_ActiveCamera].UpdateCameraPos(Amba::CameraMotion::FORWARD, AB_DeltaTime);
		}

		if (Amba::KeyBoard::Key(GLFW_KEY_W))
		{
			AB_Cameras[AB_ActiveCamera].UpdateCameraPos(Amba::CameraMotion::FORWARD, AB_DeltaTime);
		}

		if (Amba::KeyBoard::Key(GLFW_KEY_S))
		{
			AB_Cameras[AB_ActiveCamera].UpdateCameraPos(Amba::CameraMotion::BACKWARD, AB_DeltaTime);
		}

		if (Amba::KeyBoard::Key(GLFW_KEY_D))
		{
			AB_Cameras[AB_ActiveCamera].UpdateCameraPos(Amba::CameraMotion::RIGHT, AB_DeltaTime);
		}

		if (Amba::KeyBoard::Key(GLFW_KEY_A))
		{
			AB_Cameras[AB_ActiveCamera].UpdateCameraPos(Amba::CameraMotion::LEFT, AB_DeltaTime);
		}

		if (Amba::KeyBoard::Key(GLFW_KEY_SPACE))
		{
			AB_Cameras[AB_ActiveCamera].UpdateCameraPos(Amba::CameraMotion::UP, AB_DeltaTime);
		}

		if (Amba::KeyBoard::Key(GLFW_KEY_LEFT_CONTROL))
		{
			AB_Cameras[AB_ActiveCamera].UpdateCameraPos(Amba::CameraMotion::DOWN, AB_DeltaTime);
		}

		// mouse movement
		double dx = Amba::Mouse::GetDX();
		double dy = Amba::Mouse::GetDY();
		
		p_Interface->UpdateMouseD(dx, dy);

		if (!Amba::Mouse::isMouseLocked())
		{
			if (dx != 0 || dy != 0)
			{
				AB_Cameras[AB_ActiveCamera].UpdateCameraDirection(dx, dy);
			}
		}

		// zoom
		double scrollDy = Amba::Mouse::GetScrollDY();
		if (scrollDy != 0)
		{
			AB_Cameras[AB_ActiveCamera].UpdateCameraZoom(scrollDy);
		}

		// lock/unlock mouse
		if (Amba::KeyBoard::KeyWentDown(GLFW_KEY_F10))
		{
			Amba::Mouse::ToggleMouseLock();
			p_Window->ToggleCursor();
		}
	}

}

