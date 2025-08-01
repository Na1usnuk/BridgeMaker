#pragma once

#define BM_START namespace BM {
#define BM_END }

#define BIT(n) (1 << n)

#ifdef BM_PLATFORM_WINDOWS
	#define BM_API
	//#include "platform/windows/windows_window.hpp"
	BM_START
	//using Window = WindowsWindow;
	BM_END
#else
	#define BM_API
#endif

#if defined(DEBUG) || defined(_DEBUG)
	#define BM_DEBUG_CODE(...)  __VA_ARGS__
	#define BM_CORE_ASSERT(x, msg, ...) if(!(x)){ BM_CORE_FATAL(msg); __debugbreak(); __VA_ARGS__}
#else
	#define BM_DEBUG_CODE(...)
	#define BM_CORE_ASSERT(x, msg, ...)
#endif

#define BM_INHERIT_APP public ::BM::Application

#define BM_APP ::BM::Application

#define BM_APP_INITIALIZE(YourApplication) \
int main(int argc, char** argv) \
{ \
	YourApplication app; \
	return app.run(argc, argv); \
}

