#pragma once

#include <GLFW/glfw3.h>

class KeyBoard {
public:
	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);


	static bool Key(int key);
	static bool KeyChanged(int key);
	static bool KeyWentUp(int key);
	static bool KeyWentDown(int key);


private:
	static bool m_Keys[];
	static bool m_KeysChanged[];


};