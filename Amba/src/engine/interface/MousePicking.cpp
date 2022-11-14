#include "MousePicking.h"

namespace Amba {

	MousePicker::MousePicker()
		: m_MouseRay(glm::vec3(0.0f)), p_Scene(nullptr), m_ScrHeight(0.0f), m_ScrWidth(0.0f)
	{
	}

	void MousePicker::SetUp(unsigned int scr_Width, unsigned int scr_Height)
	{
		m_ScrWidth = scr_Width;
		m_ScrHeight = scr_Height;
	}

	void MousePicker::UpdateMousePos(Camera& camera)
	{
		double mouseX = Mouse::GetMouseX();
		double mouseY = Mouse::GetMouseY();

		if (mouseX > 0.0f)
			int stop = 1;

		glm::vec3 normSpace = NormalizeDeviceSpace(glm::vec3((float)mouseX, (float)mouseY, 0.0f));
		glm::vec4 clipSpace = ClipSpace(normSpace);
		glm::vec4 eyeSpace = EyeSpace(clipSpace, camera);
		m_MouseRay = WorldSpace(eyeSpace, camera);

		//AB_INFO("Ray: [{0} , {1} , {2}] || CamPos: [{3} , {4} , {5}] ", 
		//	m_MouseRay.x, m_MouseRay.y, m_MouseRay.z, camera.GetCamPos().x, camera.GetCamPos().y, camera.GetCamPos().z);
	}

	// returns boolean - check if true before using entity (default will return number 0)
	EntityId MousePicker::SelectEntity(Camera& camera, bool& found)
	{
		// shoot ray
		float shootDistance = 5.0f;
		float accumDistance = 0.0f;
		float maxRayDistance = 600.0f;

		EntityId default = 0;

		glm::vec3 ray = camera.GetCamPos();
		glm::vec3 step = m_MouseRay * shootDistance;
		// shoot ray from camera into mouse direction
		while (accumDistance <= maxRayDistance)
		{
			ray = ray +  step;

			std::vector<Cell> cellsToCheck = p_Scene->GetNearbyCells(ray);

			// find all entities in cells and change their color (implement this option in shader)
			for (auto& cell : cellsToCheck)
			{
				for (auto& ent : cell.entities)
				{
					glm::vec3 &entPos = p_Scene->GetComponent<TransformComponent>(ent)->m_Position;
					float& radius = p_Scene->GetComponent<CollisionComponent>(ent)->m_Radius;

					// check if distance from mouse to entPosition is less than radius (squared to avoid heavy computation)
					float distFromCenter = (ray.x - entPos.x) * (ray.x - entPos.x) +
						(ray.y - entPos.y) * (ray.y - entPos.y) +
						(ray.z - entPos.z) * (ray.z - entPos.z);

					if (distFromCenter < (radius * radius))
					{
						found = true;
						return ent;
					}					
				}
			}
			
			accumDistance += shootDistance;
		}
		return default;
	}


	glm::vec3 MousePicker::NormalizeDeviceSpace(glm::vec3 mousePosition)
	{
		AB_ASSERT(m_ScrWidth > 0.0f, "Window's width is 0!");
		AB_ASSERT(m_ScrHeight > 0.0f, "Window's height is 0!");

		float x = (2.0f * mousePosition.x) / m_ScrWidth - 1;
		float y = (2.0f * mousePosition.y) / m_ScrHeight - 1;
		return glm::vec3(x, y, 0.0f);
	}

	glm::vec4 MousePicker::ClipSpace(glm::vec3 mousePosition)
	{
		return glm::vec4(mousePosition.x, mousePosition.y, -1.0f, 0.0f);
	}

	glm::vec4 MousePicker::EyeSpace(glm::vec4 mousePosition, Camera& camera)
	{
		glm::mat4 InvertedProjectionMatrix = glm::inverse(camera.GetViewMatrix());
		glm::vec4 transformed = InvertedProjectionMatrix * mousePosition;
		return glm::vec4(transformed.x, transformed.y, -1.0f, 0.0f);
	}

	glm::vec3 MousePicker::WorldSpace(glm::vec4 mousePosition, Camera& camera)
	{
		// get only view matrix
		glm::mat4 view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
		glm::mat4 InvertedViewMatrix = glm::inverse(view);
		glm::vec4 transformed = InvertedViewMatrix * mousePosition;
		glm::vec3 mouseRay = glm::vec3(transformed.x, transformed.y, transformed.z);;
		mouseRay = glm::normalize(mouseRay);
		
		return mouseRay;
	}



}