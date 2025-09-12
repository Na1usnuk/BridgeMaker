module;

#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include "glm/glm.hpp"

export module bm.gfx.camera;

import bm.log;

namespace bm::gfx
{


export class Camera
{


	using vec3 = glm::vec3;
	using mat4 = glm::mat4;

public:

	Camera(float fov, float aspectRation);


	void recalculateProjection(float fov, float aspectRatio);
	void recalculateView();

	const mat4& getProjection() const { return m_projection; }
	const mat4& getView() const { return m_view; }

	void moveForward(float amount) {m_pos -= amount * m_front; }
	void moveBack(float amount) { m_pos += amount * m_front; }
	void moveLeft(float amount) {
		m_pos += glm::normalize(glm::cross(m_front, m_up)) * amount;
	}
	void moveRight(float amount) { m_pos -= glm::normalize(glm::cross(m_front, m_up)) * amount; }
	void moveUp(float amount) { m_pos.y -= amount; }
	void moveDown(float amount) { m_pos.y += amount; }


private:

	vec3 m_pos;
	vec3 m_right;
	vec3 m_up;
	vec3 m_front;

	mat4 m_view;
	mat4 m_projection;

	float m_fov;

};


}
