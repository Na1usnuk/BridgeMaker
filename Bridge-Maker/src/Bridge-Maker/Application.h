#pragma once

#include "Core.h"

BM_START

class BM_API Application
{
public:

	Application();
	virtual ~Application();

	void Run();
	
};

Application* CreateApplication();

BM_END

