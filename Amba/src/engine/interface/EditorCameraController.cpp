#include "EditorCameraController.h"

#include <engine/io/Keyboard.h>
#include <engine/io/Mouse.h>


namespace Amba {
	
	void EditorCamController::ProcessInput(glm::vec3& position, Camera* camera, double dt)
	{

		AB_ASSERT(!camera == NULL, "There is no active camera");

		bool posWasUpdated = false;

		// Camera movements - using TransformComponent
		if (Amba::KeyBoard::Key(GLFW_KEY_W))
		{
			camera->UpdateCameraPos(Amba::CameraMotion::FORWARD, dt);
			posWasUpdated = true;
		}

		if (Amba::KeyBoard::Key(GLFW_KEY_S))
		{
			camera->UpdateCameraPos(Amba::CameraMotion::BACKWARD, dt);
			posWasUpdated = true;
		}

		if (Amba::KeyBoard::Key(GLFW_KEY_D))
		{
			camera->UpdateCameraPos(Amba::CameraMotion::RIGHT, dt);
			posWasUpdated = true;
		}

		if (Amba::KeyBoard::Key(GLFW_KEY_A))
		{
			camera->UpdateCameraPos(Amba::CameraMotion::LEFT, dt);
			posWasUpdated = true;
		}

		if (Amba::KeyBoard::Key(GLFW_KEY_SPACE))
		{
			camera->UpdateCameraPos(Amba::CameraMotion::UP, dt);
			posWasUpdated = true;
		}

		if (Amba::KeyBoard::Key(GLFW_KEY_LEFT_CONTROL))
		{
			camera->UpdateCameraPos(Amba::CameraMotion::DOWN, dt);
			posWasUpdated = true;
		}

		if (posWasUpdated)
			position = camera->GetCamPos();

		// mouse movement
		double dx = Amba::Mouse::GetDX();
		double dy = Amba::Mouse::GetDY();

		if (!Amba::Mouse::isMouseLocked())
		{
			if (dx != 0 || dy != 0)
			{
				camera->UpdateCameraDirection(dx, dy);
			}
		}

		// zoom
		double scrollDy = Amba::Mouse::GetScrollDY();
		if (scrollDy != 0)
		{
			camera->UpdateCameraZoom(scrollDy);
		}
	}

}