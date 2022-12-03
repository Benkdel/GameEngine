#pragma once

#include <glm/glm.hpp>

namespace Amba {

	enum class CameraMotion {
		NONE = 0, 
		FORWARD, BACKWARD, 
		UP, DOWN, RIGHT, LEFT
	};

	class Camera
	{
	public:
		Camera(glm::vec3 pos = glm::vec3(10.0f, 5.0f, 25.0f));

		void UpdateCameraDirection(double dx, double dy);

		void UpdateCameraPos(CameraMotion direction, double dt);

		void UpdateCameraZoom(double dy);

		glm::mat4 GetViewMatrix();

		inline float GetZoom() { return m_FoV; }
		inline glm::vec3 GetCamPos() { return m_CameraPos; }
		inline glm::vec3 GetCamFront() { return m_CameraFront; }

	private:
		glm::vec3 m_CameraPos;

		glm::vec3 m_CameraFront;
		glm::vec3 m_CameraUp;
		glm::vec3 m_CameraRight;
	
		double m_Yaw; // x-axis
		double m_Pitch; // y-axis

		float m_Speed;
		float m_Sensitivity;
		float m_FoV;

		void UpdateCameraVectors();
};

}