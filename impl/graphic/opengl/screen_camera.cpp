module;

#include "glm/gtc/matrix_transform.hpp"

module bm.gfx.camera2d;


namespace bm::gfx
{
	Camera2D::Camera2D(float left, float right, float bottom, float top)
		: m_projection(glm::ortho(left, right, bottom, top, -1.f, 1.f)),
		  m_view(1.f)
	{

	}

	void Camera2D::recalculateView()
	{
		glm::mat4 
		transform = glm::translate(glm::mat4(1.f), m_pos);
		transform = glm::rotate(transform, m_rotation, glm::vec3(0.f, 0.f, 1.f));

		m_view = glm::inverse(transform);
	}
}