#pragma once

#include "core.hpp"


BM_START

class Application
{
public:

	Application();
	virtual ~Application();

	virtual int run(int argc, char** argv);
	
private:

	bool m_running;
};


BM_END

