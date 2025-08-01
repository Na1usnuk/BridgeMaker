#pragma once

#define BM_START namespace BM {
#define BM_END }

#define BIT(n) (1 << n)

#ifdef BM_PLATFORM_WINDOWS
//	#ifdef BM_BUILD_DLL
//		#define BM_API __declspec(dllexport)
//	#else
//		#define BM_API __declspec(dllimport)
//	#endif
#else
	#define BM_API
	//#error Bridge Maker only support Windows!
#endif



#if defined(DEBUG) || defined(_DEBUG)
	#define BM_DEBUG_CODE(...)  __VA_ARGS__
	#define BM_CORE_ASSERT(x, msg, ...) if(!(x)){ BM_CORE_FATAL(msg); __debugbreak(); __VA_ARGS__}
#else
	#define BM_DEBUG_CODE(...)
	#define BM_CORE_ASSERT(x, msg, ...)
#endif

