module;

#include "glad/glad.h"

export module bm.gfx.utility;

import bm.config;
import bm.log;
import std;


namespace bm::gfx 
{

export template<typename T>
concept Numeric = std::is_integral_v<T> || std::is_floating_point_v<T>;

export template<typename Container>
concept Buffer = requires (Container c)
{
	{ std::data(c) } -> std::convertible_to<const void*>;
	{ c.size() } -> std::convertible_to<std::size_t>;
	typename Container::value_type;
};

export template<typename T>
concept Data = std::is_convertible_v<T, const void*>;

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



