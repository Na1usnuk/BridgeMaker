module;

#include "stb_image.h"
#include "GLFW/glfw3.h"

export module bm.platform:window;

import std;

import bm.core;

namespace bm 
{
	
	export struct Icon
	{
		Icon(std::string_view path) : width(0), height(0), pixels(nullptr)
		{
			int bpp;
			pixels = stbi_load(path.data(), &width, &height, &bpp, 4);
			core::verify(pixels != nullptr, "Failed to load icon " + std::string(path.data()));
		}
		Icon(int w, int h, unsigned char* p) : width(w), height(h), pixels(p) {}
	
		int width;
		int height;
		unsigned char* pixels;
	};
	
	
	export class Window
	{
	public:
	
		using NativeWindow = GLFWwindow*;
		using Icon = ::bm::Icon;
		struct Data
		{
			Data(std::string_view t, int w, int h, bool vs, bool d, bool v) : title(t), width(w), height(h), vsync(vs), decorated(d), visible(v) {}
			std::string title;
			int width, height;
			bool vsync;
			bool decorated;
			bool visible;
			Window* window = nullptr;
	
			struct
			{
				int key, repeat_count;
			} last_key;
		};
	
	public:
	
		Window() = delete;
		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;
		Window(Window&& oth) noexcept;
		Window& operator=(Window&& oth) noexcept;
	
		bool operator==(const Window& oth) const { return m_window == oth.m_window; }
	
		Window(std::string_view v, int w, int h, bool vs, bool decorated = true, bool visible = true, /*to allow context make window*/ Window* shared = nullptr);
		~Window();
	
		void onUpdate();
	
		std::pair<int, int> getPosition() const;
		std::pair<int, int> getFramebufferSize() const;
		std::pair<int, int> getSize() const;
		std::pair<int, int> getFramebufferPosition() const;
		const std::string&  getTitle() const { return m_data.title; }
		int                 getWidth() const { return m_data.width; }
		int	                getHeight() const { return m_data.height; }
		bool                getVSync() const { return m_data.vsync; };
		NativeWindow        getNative() const { return m_window; }
		bool				isDecorated() const { return m_data.decorated; }
		bool				isVisible() const { return m_data.visible; }
	
		void setVSync(bool enabled);
		void setTitle(std::string_view title);
		void setIcon(Icon icon);
		void setSize(int width, int height);
		void setOpacity(float opacity = 1.f);
		void setPosition(int x, int y);
	
		void setCaptureCursor(bool value);
		
		void resize(int width, int height);
		void hide();
		void show();
	
		bool isOpen() const;
		void close();
		void destroy();
	
	private:
	
		void create(Window* shared);
	
	private:
	
		GLFWwindow* m_window;
		Data m_data;
	};
	

}