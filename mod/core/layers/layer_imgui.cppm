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

	using Ptr = std::shared_ptr<ImGuiLayer>;
	using KPtrRef = const Ptr&;

public:

	void onEvent(Event& e) override;
	void onAttach() override;
	void onDetach() override;

	//Uses ImGui naming style
	void Begin();
	void End();

};


}

