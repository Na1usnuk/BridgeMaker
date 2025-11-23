module;

#include "glm/gtc/matrix_transform.hpp"

module bm.gfx.camera.screen;


namespace bm::gfx
{
	ScreenCamera::ScreenCamera(float left, float right, float bottom, float top) :
		m_left(left), m_right(right), m_bottom(bottom), m_top(top)
	{
		recalculateView();
		recalculateProjection();
	}

	void ScreenCamera::recalculateView()
	{
		m_view = glm::mat4(1.f);
	}

	void ScreenCamera::recalculateProjection()
	{
		m_projection = glm::ortho(m_left, m_right, m_bottom, m_top, -1.f, 1.f);
	}

	const glm::mat4& ScreenCamera::getProjection() 
	{ 
		if (m_projection_dirty)
		{
			recalculateProjection();
			m_projection_dirty = false;
		}
		return m_projection; 
	}

	const glm::mat4& ScreenCamera::getView() 
	{ 
		if (m_view_dirty)
		{
			recalculateView();
			m_view_dirty = false;
		}
		return m_view; 
	}

}