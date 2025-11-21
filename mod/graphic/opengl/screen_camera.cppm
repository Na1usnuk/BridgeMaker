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

		using Ptr = std::unique_ptr<ScreenCamera>;
		using KPtrRef = const Ptr&;

	public:

		ScreenCamera(float left, float right, float bottom, float top);
		virtual ~ScreenCamera() {}

		const glm::mat4& getProjection();
		const glm::mat4& getView();

		//void setPosition(const glm::vec3& position) { m_pos = position; }
		//const glm::vec3& getPosition() const { return m_pos; }

		void setSize(float left, float right, float bottom, float top) 
		{
			m_projection = glm::ortho(left, right, bottom, top, -1.f, 1.f);
		}

		static Ptr make(float left, float right, float bottom, float top) { return std::make_unique<ScreenCamera>(left, right, bottom, top); }

	private:

		void recalculateProjection();
		void recalculateView();

	private:

		//glm::vec2 m_pos;

		glm::mat4 m_view;
		glm::mat4 m_projection;

		bool m_projection_dirty = true;
		bool m_view_dirty = true;
	};

	export using ScreenCameraPtr = Traits<ScreenCamera>::Ptr;
}