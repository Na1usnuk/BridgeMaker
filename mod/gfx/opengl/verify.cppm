export module bm.gfx:verify;

import std;

namespace bm::gfx
{
	// get Version of the current OpenGL context as an integer (e.g., 45 for version 4.5)
	int glGetVersion();

	// Clear all existing OpenGL errors
	void glClearError();

	// Log OpenGL errors if any occurred during the last function call
	void glLogOnError(std::source_location loc);

	export template<typename GLFunc, typename... ARGS>
		inline auto glVerify(std::source_location loc, GLFunc func, ARGS&&... args) -> std::invoke_result_t<GLFunc, ARGS...>
	{
		using ReturnType = std::invoke_result_t<GLFunc, ARGS...>;

		if constexpr (std::is_void_v<ReturnType>)
		{
			glClearError();
			func(std::forward<ARGS>(args)...);
			glLogOnError(loc);
		}
		else
		{
			glClearError();
			auto result = func(std::forward<ARGS>(args)...);
			glLogOnError(loc);
			return result;
		}
	}
}