module;

#include "glm/glm.hpp"

export module bm.gfx.camera2d;

import std;

namespace bm::gfx
{
	export class Camera2D
	{
	public:

		using Ptr = std::unique_ptr<Camera2D>;
		using KPtrRef = const Ptr&;

	public:

		Camera2D(float left, float right, float bottom, float top);
		virtual ~Camera2D() {}

		void recalculateProjection();
		void recalculateView();

		const glm::mat4& getProjection() const { return m_projection; }
		const glm::mat4& getView() const { return m_view; }

		void setPosition(const glm::vec3& position) { m_pos = position; }
		const glm::vec3& getPosition() const { return m_pos; }

		void setRotation(float rotation) { m_rotation = rotation; }
		float getRotation() const { return m_rotation; }

		static Ptr make(float left, float right, float bottom, float top) { return std::make_unique<Camera2D>(left, right, bottom, top); }

	private:

		glm::vec3 m_pos;
		float m_rotation;

		glm::mat4 m_view;
		glm::mat4 m_projection;

		float m_aspect_ratio;
	};

	export using Camera2DPtr = Camera2D::Ptr;
}