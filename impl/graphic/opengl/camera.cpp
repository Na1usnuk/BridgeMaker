module;

#include "glm/glm.hpp"
#include "glm/ext.hpp"

module bm.gfx.camera;

import bm.app;


namespace bm::gfx
{

	Camera::Camera(const glm::vec3& position, float fov)
		: m_pos(position),
		m_projection(1.0f),
		m_yaw(90.f),
		m_pitch(0.f),
		m_fov(fov),
		m_aspect_ratio(
			static_cast<float>(Application::get().getWindow().getWidth()) /
			static_cast<float>(Application::get().getWindow().getHeight()))
	{
		recalculateProjection();
		recalculateView();
	}

	void Camera::recalculateProjection()
	{
		m_projection = glm::perspective<float>(glm::radians(m_fov), m_aspect_ratio, 0.1f, 100.0f);
		m_projection_dirty = false;
	}

	void Camera::recalculateView()
	{
		glm::vec3 front = calculateFront();
		glm::vec3 right = calculateRight(front);
		glm::vec3 up = calculateUp(front, right);
		m_view = glm::lookAt(m_pos, m_pos + front, up);
		m_view_dirty = false;
	}

	const glm::mat4& Camera::getProjection() 
	{ 
		if (m_projection_dirty) 
		{
			recalculateProjection();
			m_projection_dirty = false;
		}
		return m_projection; 
	}

	const glm::mat4& Camera::getView() 
	{ 
		if (m_view_dirty) 
		{
			recalculateView();
			m_view_dirty = false;
		}
		return m_view;
	}

	glm::vec3 Camera::calculateFront() const
	{
		return glm::normalize(glm::vec3(std::cos(glm::radians(m_yaw)) * std::cos(glm::radians(m_pitch)),
			std::sin(glm::radians(m_pitch)),
			std::sin(glm::radians(m_yaw)) * std::cos(glm::radians(m_pitch))));
	}

	glm::vec3 Camera::calculateRight(const glm::vec3& front) const
	{
		return glm::normalize(glm::cross(front, getWorldUp()));
	}

	glm::vec3 Camera::calculateUp(const glm::vec3& front, const glm::vec3& right) const
	{
		return glm::cross(right, front);
	}

}
