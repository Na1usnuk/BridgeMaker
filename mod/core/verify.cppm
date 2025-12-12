export module bm.core:verify;

import :config;
import :log;
import :utility;
import std;

namespace bm
{
    inline void logFailure(bool is_core,
        bool condition,
        std::string_view msg,
        std::source_location loc)
    {
        auto file = std::string_view(loc.file_name());
        auto function = std::string_view(loc.function_name());
        auto line = loc.line();

        std::string full_message = std::format(
            "{} failed [file: {}; function: {}; line: {}] {}",
            is_core ? "Core verify" : "Verify",
            file,
            function,
            line,
            msg
        );

        //if (is_core)
        //    bm::log::Log::getCoreLogger()->log(spdlog::level::critical, full_message);
        //else
        //    bm::log::Log::getClientLogger()->log(spdlog::level::critical, full_message);

        debugBreak();
    }


namespace core
{

	export template<typename... Args>
	void verify(const bool condition, std::string_view msg = {}, Args&&... args, std::source_location loc = std::source_location::current())
	{
        if constexpr (config::enable_verify)
        {
            if (condition) [[likely]]
				return;
            
            std::string formatted_msg = std::string(msg);
            if constexpr (sizeof...(args) > 0)
                formatted_msg = std::vformat(msg, std::make_format_args(std::forward<Args>(args)...));

            logFailure(true, condition, formatted_msg, loc);
            
		}
	}

}

	export template<typename... Args>
	void verify(const bool condition, std::string_view msg = {}, Args&&... args, std::source_location loc = std::source_location::current())
	{
        if constexpr (config::enable_verify)
        {
            if (condition) [[likely]]
                return;

            std::string formatted_msg = std::string(msg);
            if constexpr (sizeof...(args) > 0)
                formatted_msg = std::vformat(msg, std::make_format_args(std::forward<Args>(args)...));

            logFailure(false, condition, formatted_msg, loc);

        }
	}

}