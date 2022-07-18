#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define DEFAULT_FOV 45.0f

static glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

Camera::Camera(glm::vec3 pos)
	: m_CameraPos(pos), m_CameraFront(glm::vec3(1.0f, 0.0f, 0.0f)),
	m_Pitch(0.0f), m_Yaw(0.0f), m_Speed(2.5f), m_Sensitivity(1.0f), m_FoV(DEFAULT_FOV)
{
	UpdateCameraVectors();
}

void Camera::UpdateCameraDirection(double dx, double dy)
{
	m_Yaw += dx;
	m_Pitch += dy;

	if (m_Pitch > 89.0f)
		m_Pitch = 89.0f;
	
	if (m_Pitch < -89.0f)
		m_Pitch = -89.0f;

	UpdateCameraVectors();
}

void Camera::UpdateCameraPos(CameraMotion direction, double dt)
{
    float velocity = (float)dt * m_Speed;

    switch (direction) {
    case CameraMotion::FORWARD:
        m_CameraPos += m_CameraFront * velocity;
        break;
    case CameraMotion::BACKWARD:
        m_CameraPos -= m_CameraFront * velocity;
        break;
    case CameraMotion::RIGHT:
        m_CameraPos += m_CameraRight * velocity;
        break;
    case CameraMotion::LEFT:
        m_CameraPos -= m_CameraRight * velocity;
        break;
    case CameraMotion::UP:
        m_CameraPos += m_CameraUp * velocity;
        break;
    case CameraMotion::DOWN:
        m_CameraPos -= m_CameraUp * velocity;
        break;
    }
}

void Camera::UpdateCameraZoom(double dy)
{
    if (m_FoV >= 1.0f && m_FoV <= DEFAULT_FOV) {
        m_FoV -= dy;
    }
    else if (m_FoV < 1.0f) {
        m_FoV = 1.0f;
    }
    else { // > 45.0f
        m_FoV = DEFAULT_FOV;
    }
}

glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt(m_CameraPos, m_CameraPos + m_CameraFront, m_CameraUp);
}

void Camera::UpdateCameraVectors()
{
	// update vectors
    glm::vec3 direction;
    direction.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
    direction.y = sin(glm::radians(m_Pitch));
    direction.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
    m_CameraFront = glm::normalize(direction);

    m_CameraRight = glm::normalize(glm::cross(m_CameraFront, worldUp));
    m_CameraUp = glm::normalize(glm::cross(m_CameraRight, m_CameraFront));
}
