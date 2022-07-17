#pragma once

#include <string>

#include <GLAD/glad/glad.h>
#include <GLFW/glfw3.h>

namespace Amba {

	static bool s_GLFWInitialized = false;

	class Window
	{
	public:
		Window(const std::string& title = "", unsigned int width = 1280, unsigned int height = 760);
		~Window();

		void OnUpdate();
		void ShutDown();

		void SetWindowShouldClose();

		inline unsigned int GetWidth() const { return m_Width;  };
		inline unsigned int GetHeight() const { return m_Height; };
		inline bool WindowShouldClose() const { return m_WindowShouldClose; }
		GLFWwindow* getWindow() const { return m_Window; }

	private:
		GLFWwindow* m_Window;

		unsigned int m_Width;
		unsigned int m_Height;
		std::string m_Title;

		bool m_WindowShouldClose;
	};
}
