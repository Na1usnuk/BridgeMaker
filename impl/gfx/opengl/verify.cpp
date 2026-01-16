module;

#include "glad/glad.h"

module bm.gfx:verify;

import bm.config;
import bm.core;

import std;

namespace bm::gfx
{
	// get Version of the current OpenGL context as an integer (e.g., 45 for version 4.5)
	int glGetVersion()
	{
		static int version = 0;
		if (version == 0)
		{
			GLint version_major = 0;
			GLint version_minor = 0;
			glGetIntegerv(GL_MAJOR_VERSION, &version_major);
			glGetIntegerv(GL_MINOR_VERSION, &version_minor);
			version = version_major * 10 + version_minor;
		}
		return version;
	}

	// Clear all existing OpenGL errors
	void glClearError()
	{
		if constexpr (::bm::config::is_debug)
			while (glGetError() != GL_NO_ERROR);
	}

	// Log OpenGL errors if any occurred during the last function call
	void glLogOnError(std::source_location loc)
	{
		if constexpr (::bm::config::is_debug)
			if (GLenum error = glGetError())
			{
				auto file = std::string_view(loc.file_name());
				auto function = std::string_view(loc.function_name());
				auto line = loc.line();
				bm::core::log::fatal("OpenGL function call failed [FILE: {}; FUNCTION: {}; LINE: {}] {}", file, function, line, error);
				bm::breakpoint();
			}
	}

	//export template<typename GLFunc, typename... ARGS>
	//	inline auto glVerify(std::source_location loc, GLFunc func, ARGS&&... args) -> std::invoke_result_t<GLFunc, ARGS...>
	//{
	//	using ReturnType = std::invoke_result_t<GLFunc, ARGS...>;

	//	if constexpr (std::is_void_v<ReturnType>)
	//	{
	//		glClearError();
	//		func(std::forward<ARGS>(args)...);
	//		glLogOnError(loc);
	//	}
	//	else
	//	{
	//		glClearError();
	//		auto result = func(std::forward<ARGS>(args)...);
	//		glLogOnError(loc);
	//		return result;
	//	}
	//}
}