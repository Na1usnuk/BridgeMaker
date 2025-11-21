export module bm.layer.base;

import std;

import bm.config;
import bm.window;
import bm.event.base;
import bm.traits;

namespace bm {

export class Layer;

template<typename T>
concept DerivedFromLayer = std::is_base_of_v<Layer, T>;

export class Layer
{
public:

	Layer(std::string_view name) 
		: m_name(name)
	{}

	std::string_view getName() const { return m_name; }
	void setName(std::string_view name) { m_name = name; }

	Layer() = default;
	Layer(const Layer&) = delete;
	Layer& operator=(const Layer&) = delete;
	Layer(Layer&&) = delete;
	Layer& operator=(Layer&&) = delete;

	virtual ~Layer() {}

	bool isEnabled() const { return m_enabled; }
	void enable(bool enable = true) { m_enabled = enable; }
	void disable(bool disable = true) { enable(not disable); }

	virtual void onAttach() {}
	virtual void onDetach() {}
	virtual void onEvent(Event& e) {}
	virtual void onUpdate(float delta_time) {}
	virtual void onImGuiRender() {}

	virtual void setWindow(Window& window) {} //set window to render on

	template<DerivedFromLayer L, typename... Args>
	static Traits<L>::Ptr make(Args&&... args) { return std::make_unique<L>(std::forward<Args>(args)...); }

private:

	bool m_enabled = true;

	std::string m_name;

};


}