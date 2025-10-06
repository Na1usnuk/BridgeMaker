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

	Camera(const glm::vec3& position);
	virtual ~Camera() {}

	void recalculateProjection();
	void recalculateView();

	const glm::mat4& getProjection() const { return m_projection; }
	const glm::mat4& getView() const { return m_view; }

	void setPosition(const glm::vec3& position) { m_pos = position; }
	const glm::vec3& getPosition() const { return m_pos; }

	void setFOV(float fov) { m_fov = fov; }
	float getFOV() const { return m_fov; }

	void setAspectRatio(float ratio) { m_aspect_ratio = ratio; }
	float getAspectRatio() const { return m_aspect_ratio; }

	float getYaw() const { return m_yaw; }
	float getPitch() const { return m_pitch; }

	void setYaw(float yaw) { m_yaw = yaw; }
	void setPitch(float pitch) { m_pitch = pitch; }

	const glm::vec3& getDirection() const { return m_front; }
	const glm::vec3& getRight() const { return m_right; }
	const glm::vec3& getUp() const { return m_up; }

	template<typename T = Camera>
	static Traits<T>::Ptr make(const glm::vec3& position) { return std::make_unique<T>(position); }

private:

	glm::vec3 m_pos;
	glm::vec3 m_right;
	glm::vec3 m_up;
	glm::vec3 m_front;

	glm::mat4 m_view;
	glm::mat4 m_projection;

	float m_fov;
	float m_aspect_ratio;
	float m_yaw;
	float m_pitch;
};

export using CameraPtr = Camera::Ptr;

}
