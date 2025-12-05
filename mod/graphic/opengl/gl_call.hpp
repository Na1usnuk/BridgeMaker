

#define glCall(func, ...) glCallImpl( std::source_location::current(), func, __VA_ARGS__)