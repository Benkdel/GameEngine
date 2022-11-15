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

	static double yaw = 0.0f;
	static double pitch = 0.0f;

	void MousePicker::UpdateMousePos(Camera& camera)
	{
		double mouseX = Mouse::GetMouseX();
		double mouseY = Mouse::GetMouseY();

		glm::vec3 normSpace = NormalizeDeviceSpace(glm::vec3(mouseX, mouseY, 0.0f));
		glm::vec4 clipSpace = ClipSpace(normSpace);
		glm::vec4 eyeSpace = EyeSpace(clipSpace, camera);
		m_MouseRay = WorldSpace(eyeSpace, camera);
	}

	// returns boolean - check if true before using entity (default will return number 0)
	EntityId MousePicker::SelectEntity(Camera& camera, bool& found)
	{
		// shoot ray
		float shootDistance = 1.0f;
		float accumDistance = 0.0f;
		float maxRayDistance = 600.0f;

		EntityId default = 0;

		glm::vec3 ray = camera.GetCamPos();
		glm::vec3 step = m_MouseRay * shootDistance;
		// shoot ray from camera into mouse direction
		while (accumDistance <= maxRayDistance)
		{
			ray += step;

			std::vector<Cell> cellsToCheck = p_Scene->GetNearbyCells(ray);

			// find all entities in cells and change their color (implement this option in shader)
			for (auto& cell : cellsToCheck)
			{
				for (auto& ent : cell.entities)
				{
					glm::vec3 &entPos = p_Scene->GetComponent<TransformComponent>(ent)->m_Position;
					float& radius = p_Scene->GetComponent<CollisionComponent>(ent)->m_Radius;

					// check if distance from mouse to entPosition is less than radius
					float distFromCenter = (ray.x - entPos.x) * (ray.x - entPos.x) +
						(ray.y - entPos.y) * (ray.y - entPos.y) +
						(ray.z - entPos.z) * (ray.z - entPos.z);

					if (sqrt(distFromCenter) < radius)
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


	glm::vec3 MousePicker::NormalizeDeviceSpace(glm::vec3 mousePos)
	{
		AB_ASSERT(m_ScrWidth > 0.0f, "Window's width is 0!");
		AB_ASSERT(m_ScrHeight > 0.0f, "Window's height is 0!");

		float x = ((2.0f * mousePos.x) / m_ScrWidth) - 1;
		float y = 1 - (2.0f * mousePos.y) / m_ScrHeight; // glfw begins from the top (y = 0 is at top)
		return glm::vec3(x, y, 0.0f);
	}

	glm::vec4 MousePicker::ClipSpace(glm::vec3 mousePos)
	{
		return glm::vec4(mousePos.x, mousePos.y, -1.0f, 0.0f);
	}

	glm::vec4 MousePicker::EyeSpace(glm::vec4 mousePos, Camera& camera)
	{
		//float focalLength = 1.0f / (tanf(glm::radians(camera.GetZoom() / 2.0f)));
		//float aspectRatio = (float)m_ScrHeight / (float)m_ScrWidth;
		glm::mat4 projMatrix = glm::perspective(glm::radians(camera.GetZoom()), (float)m_ScrWidth / (float)m_ScrHeight, NEAR_PLANE, FAR_PLANE);
		glm::mat4 InvertedProjectionMatrix = glm::inverse(projMatrix);
		glm::vec4 transformed = InvertedProjectionMatrix * mousePos;
		
		return glm::vec4(transformed.x, transformed.y, -1.0f, 0.0f);
		//return glm::vec4(mousePos.x / focalLength, (mousePos.y * aspectRatio) / focalLength, 1.0f, 0.0f);
	}

	glm::vec3 MousePicker::WorldSpace(glm::vec4 mousePos, Camera& camera)
	{
		glm::mat4 InvertedViewMatrix = glm::inverse(camera.GetViewMatrix());
		glm::vec4 transformed = InvertedViewMatrix * mousePos;
		glm::vec3 mouseRay = glm::vec3(transformed.x, transformed.y, transformed.z);;
		mouseRay = glm::normalize(mouseRay);

		return mouseRay;
	}



}