#include "Application.h"
#include "engine/Window.h"

namespace Amba {

	Application::Application()
	{
		Amba::Log::Init();
		AB_TRACE("Application Created Succesfully");
	}

	Application::~Application()
	{
	}

	void Application::Init(const char* title, unsigned int width, unsigned int height)
	{
		m_Window = new Amba::Window("Resurge", 1280, 760);
	}

	// test opengl and GLFW, glad with tutorial https://www.glfw.org/docs/latest/quick_guide.html
	// OR CHECK IT IN NOTES folder

	void Application::OnUserCreate()
	{

	}

	void Application::OnUserUpdate()
	{

	}

	void Application::Run()
	{
		double currentTime = 0.0f;
		double lastTime = 0.0f;

		OnUserCreate();
	
		while (!m_Window->WindowShouldClose())
		{
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			// Compute delta time and store it as class member for user to use
			currentTime = glfwGetTime();
			AB_deltaTime = currentTime - lastTime;
			lastTime = currentTime;
		
			// method used by user
			OnUserUpdate();

			m_Window->OnUpdate();
		}

		delete m_Window;
	}
}


