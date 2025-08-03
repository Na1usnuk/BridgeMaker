#include "pch.hpp"
#include "core/application.hpp"
#include "core/log.hpp"



BM_START

Application::Application(std::string_view title, int width, int height)
	:	m_window(WindowsWindow::Data(title, width, height))
{
}


Application::~Application()
{
}


int Application::run(int argc, char** argv)
{

	while (m_window.isOpen())
	{
		onUpdate();
		m_window.onUpdate();
	}

	return EXIT_SUCCESS;
}

BM_END