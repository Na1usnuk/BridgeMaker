module;

#include "glm/glm.hpp"
#include "glm/ext.hpp"

module bm.gfx.camera;

import bm.app;


namespace bm::gfx
{

Camera::Camera(const glm::vec3& position)
	: m_pos(position), m_projection(1.0f), m_yaw(90.f), m_pitch(0.f), m_fov(45.f), m_aspect_ratio(Application::get().getWindow().getWidth() / Application::get().getWindow().getHeight())
{
	m_front = glm::normalize(glm::vec3(std::cos(glm::radians(m_yaw)) * std::cos(glm::radians(m_pitch)),
							std::sin(glm::radians(m_pitch)),
							std::sin(glm::radians(m_yaw)) * std::cos(glm::radians(m_pitch))));

	m_right = glm::normalize(glm::cross(glm::vec3(0.f, 1.f, 0.f), m_front));
	m_up = glm::cross(m_front, m_right);
	m_view = glm::lookAt(m_pos, m_pos + m_front, m_up);
	m_projection = glm::perspective<float>(glm::radians(m_fov), m_aspect_ratio, 0.1f, 100.0f);
}

void Camera::recalculateProjection()
{
	m_projection = glm::perspective<float>(glm::radians(m_fov), m_aspect_ratio, 0.1f, 100.0f);
}

void Camera::recalculateView()
{
	m_right = glm::normalize(glm::cross(m_up, m_front));
	m_front = glm::normalize(glm::vec3(std::cos(glm::radians(m_yaw)) * std::cos(glm::radians(m_pitch)),
					std::sin(glm::radians(m_pitch)),
					std::sin(glm::radians(m_yaw)) * std::cos(glm::radians(m_pitch))));
	m_view = glm::lookAt(m_pos, m_pos + m_front, m_up);
}

}
