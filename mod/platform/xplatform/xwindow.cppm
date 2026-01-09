module;

#include "stb_image.h"
#include "GLFW/glfw3.h"

export module bm.platform:window;

import std;

import bm.core;

namespace bm 
{
	
	export class Window
	{
	public:
	
		using NativeWindow = GLFWwindow*;
	
	public:
	
		Window() = delete;
		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;
		Window(Window&& oth) noexcept;
		Window& operator=(Window&& oth) noexcept;
	
		bool operator==(const Window& oth) const noexcept { return m_window == oth.m_window; }
	
		Window(std::string_view title, int width, int height, bool decorated = true, bool visible = true, /*to allow context make window*/ Window* shared = nullptr) noexcept;
		~Window();
	
		std::pair<int, int> getPosition() const noexcept;
		std::pair<int, int> getFramebufferSize() const noexcept;
		std::pair<int, int> getSize() const noexcept;
		std::pair<int, int> getFramebufferPosition() const noexcept;
		std::string_view	getTitle() const noexcept;
		int                 getWidth() const noexcept;
		int	                getHeight() const noexcept;
		NativeWindow        getNative() const noexcept;
		bool				isDecorated() const noexcept;
		bool				isVisible() const noexcept;
	
		void setTitle(std::string_view title) noexcept;
		void setSize(int width, int height) noexcept;
		void setOpacity(float opacity) noexcept;
		void setPosition(int x, int y) noexcept;
		void setCaptureCursor(bool value) noexcept;
		
		void hide() noexcept;
		void show() noexcept;
	
		bool isOpen() const noexcept;
		void close() noexcept;
	
	private:
	
		void destroy() noexcept;
		void create(Window* shared) noexcept;
	
	private:
	
		NativeWindow m_window;
	};
	

}