#pragma once

#include <engine/io/Camera.h>
#include <engine/dataTypes.h>

class TransformComponent;

namespace Amba {


	class EditorCamController
	{

	public:

		static void ProcessInput(glm::vec3& position, Camera* camera, double dt);

	private:


	};

}

