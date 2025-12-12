module;

#define FMT_UNICODE 0
#include <spdlog/spdlog.h>

export module bm.core:log;

import std;

namespace bm::log
{

class Log
{
public:

	static void init();

	static std::shared_ptr<spdlog::logger>& getClientLogger() { init(); return s_client_logger; }
	static std::shared_ptr<spdlog::logger>& getCoreLogger() { init();  return s_core_logger; }

private:

	static std::shared_ptr<spdlog::logger> s_core_logger;
	static std::shared_ptr<spdlog::logger> s_client_logger;
	static std::once_flag s_once_flag;
};

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

inline void trace(std::string_view msg) { Log::getClientLogger()->trace(msg); }
inline void info(std::string_view msg) { Log::getClientLogger()->info(msg); }
inline void warning(std::string_view msg) { Log::getClientLogger()->warn(msg); }
inline void error(std::string_view msg) { Log::getClientLogger()->error(msg); }
inline void fatal(std::string_view msg) { Log::getClientLogger()->critical(msg); }

}

//Core loogging used by engine
namespace bm::core::log
{
	export template<typename... ARGS>
		inline void trace(fmt::format_string<ARGS...> fmt, ARGS&&... args) { ::bm::log::Log::getCoreLogger()->trace(fmt, std::forward<ARGS>(args)...); }
	export template<typename... ARGS>
		inline void info(fmt::format_string<ARGS...> fmt, ARGS&&... args) { ::bm::log::Log::getCoreLogger()->info(fmt, std::forward<ARGS>(args)...); }
	export template<typename... ARGS>
		inline void warning(fmt::format_string<ARGS...> fmt, ARGS&&... args) { ::bm::log::Log::getCoreLogger()->warn(fmt, std::forward<ARGS>(args)...); }
	export template<typename... ARGS>
		inline void error(fmt::format_string<ARGS...> fmt, ARGS&&... args) { ::bm::log::Log::getCoreLogger()->error(fmt, std::forward<ARGS>(args)...); }
	export template<typename... ARGS>
		inline void fatal(fmt::format_string<ARGS...> fmt, ARGS&&... args) { ::bm::log::Log::getCoreLogger()->critical(fmt, std::forward<ARGS>(args)...); }

	inline void trace(std::string_view msg) { ::bm::log::Log::getCoreLogger()->trace(msg); }
	inline void info(std::string_view msg) { ::bm::log::Log::getCoreLogger()->info(msg); }
	inline void warning(std::string_view msg) { ::bm::log::Log::getCoreLogger()->warn(msg); }
	inline void error(std::string_view msg) { ::bm::log::Log::getCoreLogger()->error(msg); }
	inline void fatal(std::string_view msg) { ::bm::log::Log::getCoreLogger()->critical(msg); }
}

