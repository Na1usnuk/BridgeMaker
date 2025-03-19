#pragma once

#ifdef BM_PLATFORM_WINDOWS

extern BM::Application* BM::CreateApplication();

int main(int argc, char** argv)
{
	auto app = BM::CreateApplication();
	app->Run();
	delete app;
}

#endif