#include "Window.h"

#include <engine/Log.h>
#include <engine/core.h>

#include <engine/io/Keyboard.h>
#include <engine/io/Mouse.h>

#ifdef AB_ENABLE_ASSERTS
	#include <engine/OpenGlErrorChecking.h>
#endif

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

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

		m_Window = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), NULL, NULL);
		if (!m_Window)
		{
			AB_ASSERT(0, "Could not create Window!\n");
		}
		glfwMakeContextCurrent(m_Window);

		// set callbacks
		glfwSetFramebufferSizeCallback(m_Window, framebuffer_size_callback);
		glfwSetKeyCallback(m_Window, KeyBoard::KeyCallback);
		glfwSetCursorPosCallback(m_Window, Mouse::CursorPosCallback);
		glfwSetMouseButtonCallback(m_Window, Mouse::MouseButtonCallback);
		glfwSetScrollCallback(m_Window, Mouse::MouseWheelCallback);

		// hide cursor
		//glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		// initialize GLAD
		int success = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		AB_ASSERT(success, "Could not load GLAD!\n");

		// enable depth testing and blending
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);

#ifdef AB_ENABLE_ASSERTS
		// only in debug mode, enable output and 
		// set callback
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback((GLDEBUGPROC)MessageCallback, NULL);
#endif
	}

	Window::~Window() {}

	void Window::OnUpdate()
	{
		glfwSwapBuffers(m_Window);
		glfwPollEvents();
	}

	void Window::SetWindowShouldClose()
	{
		m_WindowShouldClose = true;
	}

	void Window::ShutDown()
	{
		m_WindowShouldClose = true;
		if (m_Window != NULL)
		{
			glfwDestroyWindow(m_Window);
		}
	}
}
