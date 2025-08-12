#pragma once

#include "core.hpp"
#include "timer.hpp"

#define FMT_UNICODE 0
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

#include <memory>

BM_START



class  Log
{
public:

	static void init();

	static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }

private:

	static std::shared_ptr<spdlog::logger> s_CoreLogger;
	static std::shared_ptr<spdlog::logger> s_ClientLogger;
};


BM_END


#define BM_CORE_INFO(...) ::BM::Log::GetCoreLogger()->info(__VA_ARGS__)
#define BM_CORE_WARNING(...) ::BM::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define BM_CORE_ERROR(...) ::BM::Log::GetCoreLogger()->error(__VA_ARGS__)
#define BM_CORE_FATAL(...) ::BM::Log::GetCoreLogger()->critical(__VA_ARGS__)


#define BM_INFO(...) ::BM::Log::GetClientLogger()->info(__VA_ARGS__)
#define BM_WARNING(...) ::BM::Log::GetClientLogger()->warn(__VA_ARGS__)
#define BM_ERROR(...) ::BM::Log::GetClientLogger()->error(__VA_ARGS__)
#define BM_FATAL(...) ::BM::Log::GetClientLogger()->critical(__VA_ARGS__)

	
#ifdef BM_LEVEL_TRACE
	#define BM_CORE_TRACE(...) ::BM::Log::GetCoreLogger()->trace(__VA_ARGS__)
	#define BM_TRACE(...) ::BM::Log::GetClientLogger()->trace(__VA_ARGS__)
	#define BM_CORE_TRACE_FUNCTION_START BM_CORE_TRACE("Function \"{0}\" start", (BM_FUNCTION_NAME)); Timer __BM_CORE_TRACE_FUNCTION_TIMER__; 
	#define BM_CORE_TRACE_FUNCTION_END BM_CORE_TRACE("Function \"{0}\" ends in {1}s", (BM_FUNCTION_NAME), __BM_CORE_TRACE_FUNCTION_TIMER__.getTimeFromCreation());
	#define BM_CORE_TRACE_FUNCTION(...) do{BM_CORE_TRACE_FUNCTION_START __VA_ARGS__ BM_CORE_TRACE_FUNCTION_END}while(false)
	#define BM_TIME_OF(msg, ...) do{Timer __BM_TIME_OF_TIMER__; __VA_ARGS__;  BM_CORE_TRACE("{1}: {0}s", __BM_TIME_OF_TIMER__.getTimeFromCreation(), msg);}while(false)
#else 
	#define BM_CORE_TRACE(...)
	#define BM_TRACE(...)
	#define BM_CORE_TRACE_FUNCTION_START(function_name)
	#define BM_CORE_TRACE_FUNCTION_END(function_name)
	#define BM_CORE_TRACE_FUNCTION(function_name, ...) __VA_ARGS__
	#define BM_TIME_OF(msg, ...)  __VA_ARGS__;
#endif