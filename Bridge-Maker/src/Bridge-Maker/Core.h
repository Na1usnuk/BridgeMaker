#pragma once

#define BM_START namespace BM {
#define BM_END }

#ifdef BM_PLATFORM_WINDOWS
	#ifdef BM_BUILD_DLL
		#define BM_API __declspec(dllexport)
	#else
		#define BM_API __declspec(dllimport)
	#endif
#else
	#error Bridge Maker only support Windows!
#endif

