#include "Mouse.h"

#include <engine/Log.h>

namespace Amba {

	double Mouse::m_X = 0;
	double Mouse::m_Y = 0;

	double Mouse::m_LastX = 0;
	double Mouse::m_LastY = 0;

	double Mouse::m_Dx = 0;
	double Mouse::m_Dy = 0;

	double Mouse::m_ScrollDX = 0;
	double Mouse::m_ScrollDY = 0;

	bool Mouse::m_FirstMouse = 1;

	bool Mouse::m_Buttons[GLFW_MOUSE_BUTTON_LAST] = { 0 };
	bool Mouse::m_ButtonsChanged[GLFW_MOUSE_BUTTON_LAST] = { 0 };

	bool Mouse::m_IsLock = false;

	void Mouse::CursorPosCallback(GLFWwindow* window, double x, double y)
	{
		m_X = x;
		m_Y = y;
		
		if (m_FirstMouse)
		{
			m_LastX = x;
			m_LastY = y;
			m_FirstMouse = false;
		}

		// where should this be??
		if (m_IsLock)
		{
			m_LastX = x;
			m_LastY = y;
			return;
		}

		m_Dx = x - m_LastX;
		m_Dy = m_LastY - y; // inverted

		m_LastX = x;
		m_LastY = y;
	}

	void Mouse::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
	{
		if (action != GLFW_RELEASE)
		{
			if (!m_Buttons[button])
			{
				m_Buttons[button] = true;
			}
		}
		else
		{
			m_Buttons[button] = false;
		}

		m_ButtonsChanged[button] = action != GLFW_REPEAT;
	}
	void Mouse::MouseWheelCallback(GLFWwindow* window, double dx, double dy)
	{
		m_ScrollDX = dx;
		m_ScrollDY = dy;
	}

	double Mouse::GetMouseX()
	{
		return m_X;
	}

	double Mouse::GetMouseY()
	{
		return m_Y;
	}

	double Mouse::GetDX()
	{
		double dx = m_Dx;
		m_Dx = 0;
		return dx;
	}

	double Mouse::GetDY()
	{
		double dy = m_Dy;
		m_Dy = 0;
		return dy;
	}

	double Mouse::GetScrollDX()
	{
		double dx = m_ScrollDX;
		m_ScrollDX = 0;
		return dx;
	}

	double Mouse::GetScrollDY()
	{
		double dy = m_ScrollDY;
		m_ScrollDY = 0;
		return dy;
	}

	bool Mouse::Button(int button)
	{
		return m_Buttons[button];
	}

	bool Mouse::ButtonChanged(int button)
	{
		bool ret = m_ButtonsChanged[button];
		m_ButtonsChanged[button] = false;
		return ret;
	}
	bool Mouse::ButtonWentUp(int button)
	{
		return !m_Buttons[button] && ButtonChanged(button);
	}

	bool Mouse::ButtonWentDown(int button)
	{
		return m_Buttons[button] && ButtonChanged(button);
	}

	void Mouse::ToggleMouseLock()
	{
		m_IsLock = (!m_IsLock) ? 1 : 0;
	}

}
