export module bm.gfx.camera;

export import "glm/vec3.hpp";
export import "glm/mat4x4.hpp";
export import "glm/glm.hpp";

import bm.log;
import bm.traits;

import std;

namespace bm::gfx
{
export class Camera
{
public:

	using Ptr = std::shared_ptr<Camera>;
	using KPtrRef = const Ptr&;

public:

	Camera(const glm::vec3& position, float fov = 45.f);
	virtual ~Camera() {}

	const glm::mat4& getProjection();
	const glm::mat4& getView();

	void setPosition(const glm::vec3& position) { m_pos = position; m_view_dirty = true;}
	const glm::vec3& getPosition() const { return m_pos; }
	// I dont decided yet which naming convention to use, so support both
	void setPos(const glm::vec3& position) { setPosition(position); }
	const glm::vec3& getPos() const { return getPosition(); }


	void setFOV(float fov) { m_fov = fov; m_projection_dirty = true; }
	float getFOV() const { return m_fov; }

	void setAspectRatio(float ratio) { m_aspect_ratio = ratio; m_projection_dirty = true; }
	float getAspectRatio() const { return m_aspect_ratio; }

	void setYaw(float yaw) { m_yaw = yaw; m_view_dirty = true; }
	float getYaw() const { return m_yaw; }

	void setPitch(float pitch) { m_pitch = pitch; m_view_dirty = true; }
	float getPitch() const { return m_pitch; }

	glm::vec3 getDirection() const { return calculateFront(); }

	glm::vec3 getRight() const { return calculateRight(calculateFront()); }
	// Overloaded to avoid recalculating front vector multiple times
	glm::vec3 getRight(const glm::vec3& front) const { return calculateRight(front); }

	glm::vec3 getUp() const { auto front = calculateFront(); return calculateUp(front, calculateRight(front)); }
	// Overloaded to avoid recalculating front and right vectors multiple times
	glm::vec3 getUp(const glm::vec3& front, const glm::vec3& right) const { return calculateUp(front, right); }

	// Not sure if it should be part of camera
	static constexpr glm::vec3 getWorldUp() { return glm::vec3(0, 1, 0); }

	// Factory method
	template<typename T = Camera>
	static Traits<T>::Ptr make(const glm::vec3& position) { return std::make_unique<T>(position); }

private:

	void recalculateProjection();
	void recalculateView();

	glm::vec3 calculateFront() const;
	glm::vec3 calculateRight(const glm::vec3& front) const;
	glm::vec3 calculateUp(const glm::vec3& front, const glm::vec3& right) const;

private:

	glm::vec3 m_pos;

	bool m_view_dirty = true;
	bool m_projection_dirty = true;

	glm::mat4 m_view;
	glm::mat4 m_projection;

	float m_fov;
	float m_aspect_ratio;
	float m_yaw;
	float m_pitch;
};

// Exported alias
export using CameraPtr = Camera::Ptr;

}
