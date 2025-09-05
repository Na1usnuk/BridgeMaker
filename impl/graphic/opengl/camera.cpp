module;

#include "glm/glm.hpp"
#include "glm/ext.hpp"

module bm.gfx.camera;

namespace bm
{

Camera::Camera()
	: m_pos({ 0.f, 0.f, 3.f }), m_target({ 0.f, 0.f, 0.f })
{
	m_direction = glm::normalize(m_pos - m_target);
	m_right = glm::normalize(glm::cross(vec3(0.f, 1.f, 0.f), m_direction));
	m_up = glm::cross(m_direction, m_right);
	m_look_at = glm::lookAt(m_pos, m_pos + m_direction, m_up);
}

}
