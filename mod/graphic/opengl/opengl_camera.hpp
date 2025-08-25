#pragma once 

#include "core/core.hpp"

#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"

BM_START
GL_START


class OpenGLCamera
{
public:

	using vec3 = glm::vec3;
	using mat4 = glm::mat4;

public:

	OpenGLCamera();


private:

	vec3 m_pos;
	vec3 m_target;
	vec3 m_direction;
	vec3 m_right;
	vec3 m_up;
	mat4 m_look_at;

};


GL_END
BM_END

