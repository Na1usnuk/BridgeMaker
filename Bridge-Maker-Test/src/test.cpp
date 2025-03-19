#include <Bridge_Maker.h>


class Sandbox : public BM::Application
{
public:

	Sandbox() {}
	~Sandbox() {}

};


BM::Application* BM::CreateApplication()
{
	return new Sandbox();
}