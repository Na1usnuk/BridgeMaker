#include "pch.hpp"
#include "Log.hpp"




std::shared_ptr<spdlog::logger> BM::Log::s_CoreLogger;
std::shared_ptr<spdlog::logger> BM::Log::s_ClientLogger;



void BM::Log::Init()
{
	spdlog::set_pattern("%^[%T] %n: %v%$");
	s_CoreLogger = spdlog::stdout_color_mt("Bridge Maker");
	s_CoreLogger->set_level(spdlog::level::trace);

	s_ClientLogger = spdlog::stdout_color_mt("APP");
	s_ClientLogger->set_level(spdlog::level::trace);

}

