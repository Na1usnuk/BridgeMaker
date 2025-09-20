export module bm.layer.base;

import std;

import bm.config;
import bm.window;
import bm.event.base;

namespace bm {

export class Layer;

template<typename T>
concept DerivedFromLayer = std::is_base_of_v<Layer, T>;

export class Layer
{
public:

	using Ptr = std::shared_ptr<Layer>;
	using KPtrRef = const Ptr&;

public:

	Layer(std::string_view name) 
	{
		if constexpr (config::is_debug)
			m_debug_name = name;
	}
	const std::string getName() const 
	{ 
		if constexpr (config::is_debug)
			return m_debug_name;
		else
			return {};
	}


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
	static L::Ptr make(Args&&... args) { return std::make_shared<L>(std::forward<Args>(args)...); }

private:

	bool m_enabled = true;

	std::conditional<config::is_debug, std::string, std::monostate>::type m_debug_name;

};


}