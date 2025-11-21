module;

#include "glad/glad.h"

export module bm.gfx.utility;

import bm.config;
import bm.log;
import std;


namespace bm::gfx 
{

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

	void glClearError()
	{
		if constexpr (::bm::config::is_debug)
			while (glGetError() != GL_NO_ERROR);
	}
	
	bool glLogOnError(std::source_location loc = std::source_location::current())
	{
		if constexpr (::bm::config::is_debug)
			if (GLenum error = glGetError())
			{
				//BM_CORE_FATAL("[Opengl]({0}) Function: {3}; Line: {1}; File: {2}", error, loc.line, loc.file_name, loc.function_name);
				return false;
			}
		return true;
	}
	
	export template<typename GLFunc, typename... ARGS>
	constexpr inline auto glCall(GLFunc func, ARGS&&... args) -> decltype(func(std::forward<ARGS>(args)...))
	{
		if constexpr (std::is_void_v<decltype(func(std::forward<ARGS>(args)...))>)
		{
			glClearError();
			func(std::forward<ARGS>(args)...);
			glLogOnError();
		}
		else
		{
			glClearError();
			auto result = func(std::forward<ARGS>(args)...);
			glLogOnError();
			return result;
		}
	}
	

}



