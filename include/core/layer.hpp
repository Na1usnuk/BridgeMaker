#pragma once 

#include "core.hpp"
#include "events/event.hpp"

#include <string>
#include <string_view>

namespace bm {


class Layer
{
public:

	BM_DEBUG_CODE(
		Layer(std::string_view name) : m_name(name) {}
		const std::string getName() const { return m_name; }
	)

	Layer() = default;
	Layer(const Layer&) = delete;
	Layer& operator=(const Layer&) = delete;
	Layer(Layer&&) = delete;
	Layer& operator=(Layer&&) = delete;

	virtual ~Layer() {}

	bool isEnabled() const { return m_enabled; }
	void enable(bool enable = true) { m_enabled = enable; }
	void disable(bool disable = true) { enable(!disable); }

	virtual void onAttach() = 0;
	virtual void onDetach() = 0;
	virtual void onEvent(Event& e) = 0;
	virtual void onUpdate() = 0;

	virtual void setCurrent(Window* window) = 0; //set window to render on

private:

	bool m_enabled = true;

	BM_DEBUG_CODE(
		std::string m_name;
	)
};


}