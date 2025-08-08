#pragma once 

#include "core.hpp"
#include "log.hpp"
#include "glad/glad.h"

BM_START
void __gl_clear_error();

bool __gl_log_error(const char* const func, const char* const file, int line);

BM_END

#define GLCALL(x) ::BM::__gl_clear_error(); x; BM_CORE_SIMPLE_ASSERT(::BM::__gl_log_error(#x, __FILE__, __LINE__));

