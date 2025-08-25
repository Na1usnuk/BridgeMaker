#pragma once

#include "core/core.hpp"
#include <utility>


namespace bm
{

class Window;

template<typename Backend>
class AbstractInput
{
public:

	using mpos_t = std::pair<float, float>;

public:

	static bool isPressed(int key) { return m_impl.isPressed(m_window, key); };
	static bool isReleased(int key) { return m_impl.isReleased(m_window, key); };
	static mpos_t getMousePos() { return m_mouse_pos; }
	static Window* getCurrentWindow() { return m_window; }


	//for window callbacks
	static void setCurrentWindow(Window* window) { m_window = window; }
	static void setMousePos(mpos_t pos) { m_mouse_pos = pos; }

private:

	static Backend m_impl;
	//focused window
	static Window* m_window;

	static mpos_t m_mouse_pos;

};

template<typename Backend>
Backend AbstractInput<Backend>::m_impl{};

template<typename Backend>
Window* AbstractInput<Backend>::m_window = nullptr;

template<typename Backend>
AbstractInput<Backend>::mpos_t AbstractInput<Backend>::m_mouse_pos = { 0.f, 0.f };

}