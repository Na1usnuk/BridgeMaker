#pragma once

#include "core/core.hpp"
#include <unordered_map>

struct GLFWcursor;

BM_START

class XCursor
{
public:

	//glfw cursor defines
	enum class Type
	{
		Arrow = 0x00036001,
		IBeam = 0x00036002,
		Crosshair = 0x00036003,
		Hand = 0x00036004,
		HResize = 0x00036006,
		VResize = 0x00036005,
		//diagonal right to left resize cursor (top right to bottom left)
		DRLResize = 0x00036008,
		//diagonal left to right resize cursor (top left to bottom right)
		DLRResize = 0x00036007,
		//HResize + VResize 
		AResize = 0x00036009,
		NotAllowed = 0x00036009,
	};

public:

	static void setCursor(Type type);

	static void destroy();
	static void init();

private:

	static std::unordered_map<Type, GLFWcursor*> m_cursors;
	static unsigned int m_ref_count;
};

BM_END
