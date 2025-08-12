#pragma once 

#include "core/core.hpp"
#include "core/log.hpp"
#include "glad/glad.h"

BM_START
GL_START

void __gl_clear_error();

bool __gl_log_error(const char* const func, const char* const file, int line);

GL_END
BM_END

#define GLCALL(x) ::BM::GL::__gl_clear_error(); x; BM_CORE_SIMPLE_ASSERT(::BM::GL::__gl_log_error(#x, __FILE__, __LINE__));


