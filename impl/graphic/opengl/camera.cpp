module;

#include "glm/glm.hpp"
#include "glm/ext.hpp"

module bm.gfx.camera;

import bm.app;

namespace bm::gfx
{

Camera::Camera(float fov, float aspectRatio)
	: m_pos({ 0.f, 0.f, 3.f }), m_front(glm::vec3(0.0f, 0.0f, -1.0f)), m_projection(1.0f)
{
	m_right = glm::normalize(glm::cross(vec3(0.f, 1.f, 0.f), m_front));
	m_up = glm::cross(m_front, m_right);
	m_view = glm::lookAt(m_pos, m_pos + m_front, m_up);
	m_projection = glm::perspective<float>(glm::radians(fov), aspectRatio, 0.1f, 100.0f);
}

void Camera::recalculateProjection(float fov, float aspectRatio)
{
	m_projection = glm::perspective<float>(glm::radians(fov), aspectRatio, 0.1f, 100.0f);
}

void Camera::recalculateView()
{
	m_view = glm::lookAt(m_pos, m_pos + m_front, m_up);
}

}
