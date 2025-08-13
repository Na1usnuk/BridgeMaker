#pragma once

#include "core.hpp"

BM_START

template<typename Backend>
class Input
{
public:

	static bool isPressed(int key) { return Backend::isPressed(key); }
	static bool isReleased(int key) { return Backend::isReleased(key); }

};

BM_END


