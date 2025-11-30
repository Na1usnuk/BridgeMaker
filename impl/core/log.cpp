module;

#define FMT_UNICODE 0
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

module bm.log;

import std;

std::shared_ptr<spdlog::logger> bm::log::Log::s_core_logger;
std::shared_ptr<spdlog::logger> bm::log::Log::s_client_logger;
std::once_flag bm::log::Log::s_once_flag;

void bm::log::Log::init()
{
	std::call_once(s_once_flag, []()
		{
			spdlog::set_pattern("%^[%T] %n: %v%$");
			s_core_logger = spdlog::stdout_color_mt("Bridge Maker");
			s_core_logger->set_level(spdlog::level::trace);

			s_client_logger = spdlog::stdout_color_mt("APP");
			s_client_logger->set_level(spdlog::level::trace);
		});
}
