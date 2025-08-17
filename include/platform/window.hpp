#pragma once

#include "core/core.hpp"

BM_START

template<typename Backend>
class AbstractWindow
{
public:

	using NativeWindowPtr = Backend::NativeWindowPtr;
	using Data = Backend::Data;
	using EventCallbackFn = Backend::EventCallbackFn;
	using Icon = Backend::Icon;

public:
	AbstractWindow(const AbstractWindow&) = delete;
	AbstractWindow& operator=(const AbstractWindow&) = delete;
	AbstractWindow(AbstractWindow&& oth) = delete;
	AbstractWindow& operator=(AbstractWindow&& oth) = delete;

	bool operator==(const AbstractWindow& oth) const { return m_impl == oth.m_impl; }

	AbstractWindow(std::string_view v, int w, int h, bool vs = false, bool decorated = true, bool visible = true) : AbstractWindow(Data{ v, w, h, vs }, decorated, visible) {}
	AbstractWindow(const Data& data = Data{}, bool decorated = true, bool visible = true) : m_impl(data, decorated, visible)
	{ 
		m_impl.m_self = this;  
		m_impl.m_data.window = this; 
		Backend::onFocus(getNativeWindow(), true);
	}

	void onUpdate() { m_impl.onUpdate(); }

	unsigned int		getWidth() const { return m_impl.getWidth(); }
	unsigned int		getHeight() const { return m_impl.getHeight(); }
	bool				getVSync() const { return m_impl.getVSync(); }
	NativeWindowPtr		getNativeWindow() const { return m_impl.getNativeWindow(); }
	std::pair<int, int> getPosition() const { return m_impl.getPosition(); }
	std::pair<int, int> getFramebufferSize() const { return m_impl.getFramebufferSize(); }
	std::pair<int, int> getSize() const { return m_impl.getSize(); }
	std::pair<int, int> getFramebufferPosition() const { return m_impl.getFramebufferPosition(); }
	std::string			getTitle() const { return m_impl.getTitle(); }

	void setVSync(bool enabled) { m_impl.setVSync(enabled); }
	void setEventCallback(EventCallbackFn fn) { m_impl.setEventCallback(fn); }
	void setTitle(std::string_view title) { m_impl.setTitle(title); }
	void setIcon(Icon icon) { m_impl.setIcon(icon); }
	void setSize(int width, int height) { m_impl.setSize(width, height); }
	void setOpacity(float opacity = 1.f) { m_impl.setOpacity(opacity); }
	void setPosition(int x, int y) { m_impl.setPosition(x, y); }
	void resize(int width, int height) { m_impl.resize(width, height); }

	void hide() { m_impl.hide(); }
	void show() { m_impl.show(); }
	bool isOpen() const { return m_impl.isOpen(); }
	void close() { m_impl.close(); }
	void destroy() { m_impl.destroy(); }

	Backend& getImpl() { return m_impl; }

private:

	Backend m_impl;

};

BM_END
