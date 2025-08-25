#pragma once

#include "core/core.hpp"

namespace bm
{

template<typename Backend>
class AbstractCursor
{
public:

	using Type = Backend::Type;

public:

	static void setCursor(Type type = Type::Arrow) { m_impl.setCursor(type); }
	static void init() { m_impl.init(); }
	static void destroy() { m_impl.destroy(); }


private:

	static Backend m_impl;

};

template<typename Backend>
Backend AbstractCursor<Backend>::m_impl{};

}

