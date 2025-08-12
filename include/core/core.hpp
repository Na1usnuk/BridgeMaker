#pragma once

#define BM_START namespace BM {
#define BM_END }

#define BIT(n) (1 << n)

#define BM_BIND_EVENT_FN(func_name) std::bind(&func_name, this, std::placeholders::_1)

#ifdef _MSC_VER
	#define BM_FUNCTION_NAME __FUNCSIG__
#elif defined(__GNUC__)
	#define BM_FUNCTION_NAME __PRETTY_FUNCTION__
#else
	#define BM_FUNCTION_NAME __func__
#endif //BM_FUNCTION_NAME 

#if defined(DEBUG) || defined(_DEBUG)
	#define BM_DEBUG_CODE(...)  __VA_ARGS__
	#define BM_CORE_ASSERT(x, msg, ...) if(!(x)){ BM_CORE_FATAL(msg); __debugbreak(); __VA_ARGS__}
	#define BM_CORE_SIMPLE_ASSERT(x) if(!(x)){__debugbreak();}
//	#define BM_LEVEL_TRACE
#else
	#define BM_DEBUG_CODE(...)
	#define BM_CORE_ASSERT(x, msg, ...)
	#define BM_CORE_SIMPLE_ASSERT(x) 
#endif //DEBUG

#define BM_APP ::BM::Application

//important pre start initalization
#define BM_INIT  ::BM::Log::init();


//simple main function template.
//can be overriden, but dont forget to return app.run(argc, argv) and write BM_INIT in beginning
#define BM_APP_INITIALIZE(YourApplication) \
int main(int argc, char** argv) \
{ \
	BM_INIT \
	YourApplication app; \
	return app.run(argc, argv); \
}


#define GL_START namespace OpenGL {
#define GL_END }
