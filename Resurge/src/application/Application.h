#pragma once

#include "engine/Window.h"
#include "engine/Log.h"

namespace Amba {

class Application
{
	public:
		Application();
		~Application();

		void Run();

		void Init(const char* title = "Game", unsigned int width = 1280, unsigned int height = 760);

		// methods to be overrided by user
		virtual void OnUserCreate();
		virtual void OnUserUpdate();

	private:
		Amba::Window* m_Window;

	protected:
		double AB_deltaTime;


	};
}

