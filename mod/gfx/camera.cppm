export module bm.gfx:camera;

import std;
import glm;

import bm.core;


namespace bm::gfx
{
	export class Camera
	{
	public:
	
		Camera(const glm::vec3& position, float fov, float aspect_ratio);
	
		const glm::mat4& getProjection();
		const glm::mat4& getView();
		const glm::mat4& getViewProjection();
	
		void setPosition(const glm::vec3& position) { m_pos = position; m_view_dirty = m_view_projection_dirty = true;}
		const glm::vec3& getPosition() const { return m_pos; }	
	
		void setFOV(float fov) { m_fov = fov; m_projection_dirty = m_view_projection_dirty = true;}
		float getFOV() const { return m_fov; }
	
		void setAspectRatio(float ratio) { m_aspect_ratio = ratio; m_projection_dirty = m_view_projection_dirty = true;}
		float getAspectRatio() const { return m_aspect_ratio; }
	
		void setYaw(float yaw) { m_yaw = yaw; m_view_dirty = m_view_projection_dirty = true;}
		float getYaw() const { return m_yaw; }
	
		void setPitch(float pitch) { m_pitch = pitch; m_view_dirty = m_view_projection_dirty = true;}
		float getPitch() const { return m_pitch; }
	
		glm::vec3 getDirection() const { return calculateFront(); }
	
		glm::vec3 getRight() const { return calculateRight(calculateFront()); }
		// Overloaded to avoid recalculating front vector multiple times
		glm::vec3 getRight(const glm::vec3& front) const { return calculateRight(front); }
	
		glm::vec3 getUp() const { auto front = calculateFront(); return calculateUp(front, calculateRight(front)); }
		// Overloaded to avoid recalculating front and right vectors multiple times
		glm::vec3 getUp(const glm::vec3& front, const glm::vec3& right) const { return calculateUp(front, right); }
	

		static constexpr glm::vec3 getWorldUp() { return glm::vec3(0, 1, 0); }

	private:
	
		void recalculateProjection();
		void recalculateView();
		void recalculateViewProjection();
	
		glm::vec3 calculateFront() const;
		glm::vec3 calculateRight(const glm::vec3& front) const;
		glm::vec3 calculateUp(const glm::vec3& front, const glm::vec3& right) const;
	
	private:
	
		glm::vec3 m_pos;
	
		bool m_view_dirty = true;
		bool m_projection_dirty = true;
		bool m_view_projection_dirty = true;
	
		glm::mat4 m_view;
		glm::mat4 m_projection;
		glm::mat4 m_view_proj;
	
		float m_fov;
		float m_aspect_ratio;
		float m_yaw;
		float m_pitch;
	};


	export class ScreenCamera
	{
	public:

		ScreenCamera(float left, float right, float bottom, float top);
		ScreenCamera(float width, float height) : ScreenCamera(0.f, width, 0.f, height) {}

		const glm::mat4& getProjection();
		const glm::mat4& getView();

		//void setPosition(const glm::vec3& position) { m_pos = position; }
		//const glm::vec3& getPosition() const { return m_pos; }

		void setViewport(float left, float right, float bottom, float top);
		void setViewportSize(float width, float height) { setViewport(0.f, width, 0.f, height); }


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
