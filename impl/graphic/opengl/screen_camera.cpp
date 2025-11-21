module;

#include "glm/gtc/matrix_transform.hpp"

module bm.gfx.camera.screen;


namespace bm::gfx
{
	ScreenCamera::ScreenCamera(float left, float right, float bottom, float top)
		: m_projection(glm::ortho(left, right, bottom, top, -1.f, 1.f)),
		  m_view(1.f)
	{

	}

	void ScreenCamera::recalculateView()
	{
		//m_view = glm::mat4(1.f);
	}

	void ScreenCamera::recalculateProjection()
	{
		//
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