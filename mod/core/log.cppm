module;

#define FMT_UNICODE 0
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include <spdlog/fmt/fmt.h>

export module bm.log;

import std;

namespace bm::log
{

export class  Log
{
public:

	static void init();

	static std::shared_ptr<spdlog::logger>& getClientLogger() { return s_client_logger; }
	static std::shared_ptr<spdlog::logger>& getCoreLogger() { return s_core_logger; }

private:

	static std::shared_ptr<spdlog::logger> s_core_logger;
	static std::shared_ptr<spdlog::logger> s_client_logger;
};





//Core loogging used by engine
namespace core
{
	export template<typename... ARGS>
		inline void trace(fmt::format_string<ARGS...> fmt, ARGS&&... args) { Log::getCoreLogger()->trace(fmt, std::forward<ARGS>(args)...); }
	export template<typename... ARGS>
		inline void info(fmt::format_string<ARGS...> fmt, ARGS&&... args) { Log::getCoreLogger()->info(fmt, std::forward<ARGS>(args)...); }
	export template<typename... ARGS>
		inline void warning(fmt::format_string<ARGS...> fmt, ARGS&&... args) { Log::getCoreLogger()->warn(fmt, std::forward<ARGS>(args)...); }
	export template<typename... ARGS>
		inline void error(fmt::format_string<ARGS...> fmt, ARGS&&... args) { Log::getCoreLogger()->error(fmt, std::forward<ARGS>(args)...); }
	export template<typename... ARGS>
		inline void fatal(fmt::format_string<ARGS...> fmt, ARGS&&... args) { Log::getCoreLogger()->critical(fmt, std::forward<ARGS>(args)...); }
}

//Logging to use by client
export template<typename... ARGS>
	inline void trace(fmt::format_string<ARGS...> fmt, ARGS&&... args) { Log::getClientLogger()->trace(fmt, std::forward<ARGS>(args)...); }
export template<typename... ARGS>
	inline void info(fmt::format_string<ARGS...> fmt, ARGS&&... args){Log::getClientLogger()->info(fmt, std::forward<ARGS>(args)...);}
export template<typename... ARGS>
	inline void warning(fmt::format_string<ARGS...> fmt, ARGS&&... args){Log::getClientLogger()->warn(fmt, std::forward<ARGS>(args)...);}
export template<typename... ARGS>
	inline void error(fmt::format_string<ARGS...> fmt, ARGS&&... args){Log::getClientLogger()->error(fmt, std::forward<ARGS>(args)...);}
export template<typename... ARGS>
	inline void fatal(fmt::format_string<ARGS...> fmt, ARGS&&... args){Log::getClientLogger()->critical(fmt, std::forward<ARGS>(args)...);}







}

