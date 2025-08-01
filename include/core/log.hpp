#pragma once

#include "Core.hpp"

#define FMT_UNICODE 0
#include "spdlog/spdlog.hpp"
#include "spdlog/sinks/stdout_color_sinks.hpp"

BM_START



class BM_API Log
{
public:

	static void Init();

	static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }

private:

	static std::shared_ptr<spdlog::logger> s_CoreLogger;
	static std::shared_ptr<spdlog::logger> s_ClientLogger;
};


BM_END


#define BM_CORE_TRACE(...) ::BM::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define BM_CORE_INFO(...) ::BM::Log::GetCoreLogger()->info(__VA_ARGS__)
#define BM_CORE_WARNING(...) ::BM::Log::GetCoreLogger()->warning(__VA_ARGS__)
#define BM_CORE_ERROR(...) ::BM::Log::GetCoreLogger()->error(__VA_ARGS__)
#define BM_CORE_FATAL(...) ::BM::Log::GetCoreLogger()->critical(__VA_ARGS__)


#define BM_TRACE(...) ::BM::Log::GetClientLogger()->trace(__VA_ARGS__)
#define BM_INFO(...) ::BM::Log::GetClientLogger()->info(__VA_ARGS__)
#define BM_WARNING(...) ::BM::Log::GetClientLogger()->warning(__VA_ARGS__)
#define BM_ERROR(...) ::BM::Log::GetClientLogger()->error(__VA_ARGS__)
#define BM_FATAL(...) ::BM::Log::GetClientLogger()->critical(__VA_ARGS__)