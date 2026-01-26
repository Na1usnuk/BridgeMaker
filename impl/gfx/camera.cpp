module bm.gfx:camera;

import :camera;

import glm;
import bm.platform;


namespace bm::gfx
{

	// Camera

	Camera::Camera(const glm::vec3& position, float fov, float aspect_ratio)
		: m_pos(position),
		m_projection(1.f),
		m_view(1.f),
		m_yaw(90.f),
		m_pitch(0.f),
		m_fov(fov),
		m_aspect_ratio(aspect_ratio)
	{
		recalculateViewProjection();
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

	void Camera::recalculateViewProjection()
	{
		m_view_proj = getProjection() * getView();
		m_view_projection_dirty = false;
	}

	const glm::mat4& Camera::getViewProjection()
	{
		if (m_view_projection_dirty or m_view_dirty or m_projection_dirty)
			recalculateViewProjection();
		return m_view_proj;
	}

	const glm::mat4& Camera::getProjection() 
	{ 
		if (m_projection_dirty) 
			recalculateProjection();
		return m_projection; 
	}

	const glm::mat4& Camera::getView() 
	{ 
		if (m_view_dirty) 
			recalculateView();
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



	// Screen Camera

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

	void ScreenCamera::setViewport(float left, float right, float bottom, float top)
	{
		m_left = left;
		m_right = right;
		m_bottom = bottom;
		m_top = top;
		m_projection_dirty = true;
	}

}
