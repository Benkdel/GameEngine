#include "Window.h"
#include "Log.h"
#include "core.h"

namespace Amba {

	void framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}

	Window::Window(const std::string& title, unsigned int width, unsigned int height)
	{
		m_Width = width;
		m_Height = height;
		m_Title = title;

		m_WindowShouldClose = false;

		AB_INFO("Creating window {0} ({1}, {2})", title, width, height);
	
		if (!s_GLFWInitialized)
		{
			int success = glfwInit();
			AB_ASSERT(success, "Could not initialize GLFW!\n");

			s_GLFWInitialized = true;
		}

		m_Window = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), NULL, NULL);
		if (!m_Window)
		{
			AB_ASSERT(0, "Could not create Window!\n");
		}
		glfwMakeContextCurrent(m_Window);

		glfwSetFramebufferSizeCallback(m_Window, framebuffer_size_callback);
		
		//initialize GLAD
		int success = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		AB_ASSERT(success, "Could not load GLAD!\n");
	}

	Window::~Window() {}

	void Window::OnUpdate()
	{
		glfwPollEvents();
		glfwSwapBuffers(m_Window);

		ProcessInput();
	}

	void Window::ShutDown()
	{
		m_WindowShouldClose = true;
		if (m_Window != NULL)
		{
			glfwDestroyWindow(m_Window);
		}
	}

	void Window::ProcessInput()
	{
		if (glfwGetKey(m_Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			ShutDown();
	}

}
