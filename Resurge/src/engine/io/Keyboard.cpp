#include "Keyboard.h"

#include <iostream>


bool KeyBoard::m_Keys[GLFW_KEY_LAST] = { 0 };
bool KeyBoard::m_KeysChanged[GLFW_KEY_LAST] = { 0 };

void KeyBoard::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action != GLFW_RELEASE)
	{
		if (!m_Keys[key])
		{
			m_Keys[key] = true;
		}
	}
	else
	{
		m_Keys[key] = false;
	}

	m_KeysChanged[key] = action != GLFW_REPEAT;
}

bool KeyBoard::Key(int key) 
{
	return m_Keys[key];
}

bool KeyBoard::KeyChanged(int key)
{
	bool ret = m_KeysChanged[key];
	m_KeysChanged[key] = false;
	return ret;
}

bool KeyBoard::KeyWentUp(int key) 
{
	return !m_Keys[key] && KeyChanged(key);
}

bool KeyBoard::KeyWentDown(int key)
{
	return m_Keys[key] && KeyChanged(key);
}
