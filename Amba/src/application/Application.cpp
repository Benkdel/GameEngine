#include "Application.h"
#include "engine/Window.h"

#include <imgui.h>

#include <engine/interface/inferface.h>

namespace Amba {

	Application::Application()
		: AB_DeltaTime(0.0f), AB_TimeElapsed(0.0f), m_Window(NULL),
		m_ScrWidth(1280), m_ScrHeight(760)
	{
		Amba::Log::Init();
		AB_TRACE("Application Created Succesfully");
	}

	Application::~Application()
	{
	}

	void Application::Init(const char* title, unsigned int width, unsigned int height)
	{
		m_Window = new Amba::Window("Resurge", width, height);
		m_ScrWidth = width;
		m_ScrHeight = height;

		// for now, init Imgui here
		// Setup Dear ImGui context
		m_Interface = new Interface();
		m_Interface->Setup(m_Window);

		// set active camera to 0 - default camera
		AB_ActiveCamera = 0;
	}

	// method to be override by user
	void Application::OnUserCreate()
	{
	}

	// method to be override by user
	void Application::OnUserUpdate()
	{
	}

	bool Application::AB_keyPressed(unsigned int GLFW_KEY_CODE)
	{
		if (glfwGetKey(m_Window->getWindow(), GLFW_KEY_CODE) == GLFW_PRESS)
			return true;
		else
			return false;
	}

	bool Application::AB_keyReleased(unsigned int GLFW_KEY_CODE)
	{
		if (glfwGetKey(m_Window->getWindow(), GLFW_KEY_CODE) == GLFW_RELEASE)
			return true;
		else
			return false;
	}

	void Application::InterfaceHandler()
	{
		m_Interface->Run();
	}

	void Application::SetWindowShouldClose()
	{
		m_Window->SetWindowShouldClose();
	}

	void Application::Run()
	{
		double lastTime = 0.0f;

		// method used by user
		OnUserCreate();
	
		while (!m_Window->WindowShouldClose())
		{
			Renderer::Clear(m_Interface->m_Clear_color);

			// Compute delta time and store it as class member for user to use
			AB_TimeElapsed = glfwGetTime();
			AB_DeltaTime = AB_TimeElapsed - lastTime;
			lastTime = AB_TimeElapsed;
		
			// method used by user
			OnUserUpdate();

			// run interface - ImGui
			InterfaceHandler();

			m_Window->OnUpdate();

			ProcessInput();
		}

		delete m_Window;
		m_Interface->Cleanup();
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
		if (dx != 0 || dy != 0)
		{
			AB_Cameras[AB_ActiveCamera].UpdateCameraDirection(dx, dy);
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
			m_Window->ToggleCursor();
		}
	}

}

