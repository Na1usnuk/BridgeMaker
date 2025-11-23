module;

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

export module bm.gfx.camera.screen;

import bm.traits;

import std;

namespace bm::gfx
{
	export class ScreenCamera
	{
	public:

		ScreenCamera(float left, float right, float bottom, float top);
		ScreenCamera(float width, float height) : ScreenCamera(0.f, width, 0.f, height) {}
		virtual ~ScreenCamera() {}

		const glm::mat4& getProjection();
		const glm::mat4& getView();

		//void setPosition(const glm::vec3& position) { m_pos = position; }
		//const glm::vec3& getPosition() const { return m_pos; }

		void setViewport(float left, float right, float bottom, float top) 
		{
			m_left = left;
			m_right = right;
			m_bottom = bottom;
			m_top = top;
			m_projection_dirty = true;
		}

		void setViewportSize(float width, float height)
		{
			setViewport(0.f, width, 0.f, height);
		}

		template<typename... Args>
		static Traits<ScreenCamera>::Ptr make(Args&&... args) { return std::make_unique<ScreenCamera>(std::forward<Args>(args)...); }

	private:

		void recalculateProjection();
		void recalculateView();

	private:

		//glm::vec2 m_pos;

		glm::mat4 m_view;
		glm::mat4 m_projection;

		float m_left, m_right, m_bottom, m_top;

		bool m_projection_dirty = true;
		bool m_view_dirty = true;
	};
}