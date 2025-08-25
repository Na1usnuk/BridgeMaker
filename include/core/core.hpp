#pragma once
#define BM_PLATFORM_X


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
	#define BM_LEVEL_TRACE
#else
	#define BM_DEBUG_CODE(...)
	#define core::verify(x, msg, ...)
	#define BM_CORE_SIMPLE_ASSERT(x) 
#endif //DEBUG

#define BM_APP ::bm::Application

//important pre start initalization
#define BM_INIT  ::bm::Log::init();


//simple main function template.
//can be overriden, but dont forget to return app.run(argc, argv) and write BM_INIT in beginning
#define BM_APP_INIT(YourApplication) \
int main(int argc, char** argv) \
{ \
	BM_INIT \
	return YourApplication::getApplication().run(argc, argv); \
}



namespace bm {
template<typename Backend> class AbstractInput;
template<typename Backend> class AbstractCursor;
}

#ifdef BM_PLATFORM_X
namespace bm {
namespace gfx {
class OpenGLRenderer;
class OpenGLContext;
}
class XInput;
class XCursor;
using Input = ::bm::AbstractInput<::bm::XInput>;
using Cursor = ::bm::AbstractCursor<::bm::XCursor>;
}
#endif