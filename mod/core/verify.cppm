export module bm.core:verify;

import bm.config;

import :log;
import :utility;
import std;

namespace bm::core
{
    export inline void breakpoint()
    {
#if defined(_MSC_VER)
        __debugbreak();
#elif defined(__clang__)
        __builtin_debugtrap();
#elif defined(__GNUC__)
        __builtin_trap();
#else
        * (volatile int*)0 = 0;
#endif
    }

    void logFailure(bool is_core, std::string_view msg, std::source_location loc = std::source_location{})
    {
        using namespace std::string_literals;
        using namespace std::string_view_literals;
        std::string loc_msg{ "" };
		if (std::string_view is_meaningles = loc.file_name(); not is_meaningles.empty())
        {
			std::string_view file = loc.file_name();
			std::size_t line_num = loc.line();
			std::string_view function = loc.function_name();
            loc_msg = std::vformat(
                "[FILE: {}; FUNCTION: {}; LINE: {}]"sv,
                std::make_format_args(
                    file,
                    function,
                    line_num
                )
            );
        }

		std::string full_message = 
            std::vformat("!VERIFY FAILED! {} {}"s,
                std::make_format_args(msg, loc_msg));

        if (is_core)
            log::Log::getCoreLogger()->log(spdlog::level::critical, full_message);
        else
            log::Log::getClientLogger()->log(spdlog::level::critical, full_message);
    }


	export template<typename... Args>
	void verify(std::source_location loc, const bool condition, std::string_view msg = {}, Args&&... args)
	{
        if constexpr (config::enable_verify)
        {
            if (condition) [[likely]]
                return;

            std::string formatted_msg = std::string(msg);
            if constexpr (sizeof...(args) > 0)
                formatted_msg = std::vformat(msg, std::make_format_args(std::forward<Args>(args)...));

            using namespace std;
            if constexpr (config::is_debug)
            {
                logFailure(false, formatted_msg, loc);
                breakpoint();
            }
            else
                terminate();
        }
	}

    export template<typename... Args>
    void verify(const bool condition, std::string_view msg = {}, Args&&... args)
    {
        verify(std::source_location{}, condition, msg, std::forward<Args>(args)...);
    }
}