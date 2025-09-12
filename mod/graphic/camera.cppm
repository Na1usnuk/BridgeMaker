module;

#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"

export module bm.gfx.camera;


namespace bm
{


export class Camera
{
public:

	using vec3 = glm::vec3;
	using mat4 = glm::mat4;

public:

	Camera();


private:

	vec3 m_pos;
	vec3 m_target;
	vec3 m_direction;
	vec3 m_right;
	vec3 m_up;
	mat4 m_look_at;

};


}
