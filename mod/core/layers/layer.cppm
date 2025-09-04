export module bm.layer.base;

import std;

import bm.config;
import bm.window;
import bm.event.base;

namespace bm {


export class Layer
{
public:

	Layer(std::string_view name) 
	{
		if constexpr (config::is_debug)
			m_debug_name = name;
	}
	const std::string getName() const 
	{ 
		if constexpr (config::is_debug)
			return m_name;
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

	virtual void onAttach() = 0;
	virtual void onDetach() = 0;
	virtual void onEvent(Event& e) = 0;
	virtual void onUpdate() = 0;

	virtual void setCurrent(Window& window) = 0; //set window to render on

private:

	bool m_enabled = true;

	std::string m_name;
	std::conditional<config::is_debug, std::string, std::monostate>::type m_debug_name;

};


}