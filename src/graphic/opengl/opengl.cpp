#include "pch.hpp"
#include "opengl/opengl.hpp"

BM_START
GL_START

void __gl_clear_error()
{
	while (glGetError() != GL_NO_ERROR);
}

bool __gl_log_error(const char* const func, const char* const file, int line)
{
	if (GLenum error = glGetError())
	{
		BM_CORE_FATAL("[Opengl]({0}) Function: {3}; Line: {1}; File: {2}", error, line, file, func);
		return false;
	}
	return true;
}

GL_END
BM_END