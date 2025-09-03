export module bm.assert;

import bm.config;
import bm.log;
import std;

namespace bm
{
	void verifyImpl(const bool condition, std::string_view msg, const bool is_core, std::source_location loc = std::source_location::current())
	{
		std::string_view file = loc.file_name();
		std::string_view function = loc.function_name();
		auto line = loc.line();

		if constexpr (config::is_debug)
			if (not condition)
			{
				is_core ? 
					log::core::fatal("Core verify failed [file: {0}; function: {1}; line: {2}] {3}", file, function, line, msg)
				  : log::fatal("Verify failed [File: {0}; Function: {1}; Line: {2}] {3}", file, function, line, msg);

				if (true/*std::is_debugger_present()*/) // waiting for C++26 debugging header
					/*std::breakpoint();*/
					__debugbreak(); 
				else
					std::terminate();
			}
	}


namespace core
{
	export void verify(const bool condition, std::string_view msg = {})
	{
		verifyImpl(condition, msg, true);
	}

	//just for consistensy with assert
	export template<bool condition = false>
		void verify(std::string_view msg = {}) { verify(condition, msg); }

	export template<bool condition = false>
		void assert(/*std::string_view msg = {}*/) //waits for C++26
	{
		if constexpr (std::is_constant_evaluated())
			static_assert(condition, /*msg*/ "Assertion failed");
	}

} //namespace core

	//wrapper to static_assert to work only in debug
	export template<bool condition = false>
		decltype(::bm::core::assert<condition>) assert = &::bm::core::assert<condition>;

	//just for consistensy with assert
	//like assert but works in runtime (debug only)
	export template<bool condition = false>
		void verify(std::string_view msg = {}){ verify(condition, msg); }
	//like assert but works in runtime (debug only)
	export void verify(const bool condition, std::string_view msg = {})
	{
		verifyImpl(condition, msg, false);
	}



} //namespace bm