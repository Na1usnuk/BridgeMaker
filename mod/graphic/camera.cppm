module;

#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"

export module bm.gfx.camera;


namespace bm::gfx
{


export class Camera
{
public:

	using vec3 = glm::vec3;
	using mat4 = glm::mat4;

public:

	Camera(float fov, float aspectRation);


	void recalculateProjection(float aspectRatio);
	void recalculateView();

	const mat4& getProjection() const { return m_projection; }
	const mat4& getView() const { return m_view; }

	//void moveForward(float amount) { }

private:

	vec3 m_pos;
	vec3 m_target;
	vec3 m_direction;
	vec3 m_right;
	vec3 m_up;
	mat4 m_view;
	mat4 m_projection;

	float m_fov;

};


}
