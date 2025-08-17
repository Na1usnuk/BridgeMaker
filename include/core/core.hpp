#pragma once
#define BM_PLATFORM_X

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
	#define BM_LEVEL_TRACE
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
#define BM_APP_INIT(YourApplication) \
int main(int argc, char** argv) \
{ \
	BM_INIT \
	return YourApplication::getApplication().run(argc, argv); \
}


#define GL_START namespace GL {
#define GL_END }

BM_START
template<typename Backend> class AbstractRenderer;
template<typename Backend> class AbstractContext;
template<typename Backend> class AbstractWindow;
template<typename Backend> class AbstractInput;
template<typename Backend> class AbstractCursor;
BM_END

#ifdef BM_PLATFORM_X
BM_START
GL_START
class OpenGLRenderer;
class OpenGLContext;
GL_END
class XWindow;
class XInput;
class XCursor;
using Renderer = ::BM::AbstractRenderer<::BM::GL::OpenGLRenderer>;
using Context = ::BM::AbstractContext<::BM::GL::OpenGLContext>;
using Window = ::BM::AbstractWindow<::BM::XWindow>;
using Input = ::BM::AbstractInput<::BM::XInput>;
using Cursor = ::BM::AbstractCursor<::BM::XCursor>;
BM_END
#endif