#include "pch.hpp"
#include "core/application.hpp"
#include "core/log.hpp"



BM_START

Application::Application()
	: m_running(true)
{
}


Application::~Application()
{
}


int Application::run(int argc, char** argv)
{
	while (m_running)
	{
	}

	return EXIT_SUCCESS;
}

BM_END