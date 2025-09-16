export module bm.layer.imgui;

import std;

import bm.layer.base;
import bm.event;
import bm.window;

export import "imgui.h";
export import "imgui_impl_opengl3.h";
export import "imgui_impl_glfw.h";

namespace bm {


export class ImGuiLayer : public Layer
{
public:

	void onEvent(Event& e) override;
	void onAttach() override;
	void onDetach() override;

	//Use CamelCase naming style to match ImGui function names
	void Begin();
	void End();

};


}

