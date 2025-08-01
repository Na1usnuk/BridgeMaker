#pragma once

#include "core/core.hpp"

#include <string>
#include <string_view>
#include <memory>


BM_START


class AbstractWindow
{
public:
	

	//using EventCallbackFn = std::function<void(Event&)>;

public:

	virtual ~AbstractWindow() {}

	virtual void onUpdate() = 0;

	virtual unsigned int getWidth() const = 0;
	virtual unsigned int getHeight() const = 0;
	virtual bool getVSync() const = 0;

	virtual void setVSync(bool enabled) = 0;

	virtual bool isOpen() const = 0;
	virtual void close() = 0;

	struct Data
	{
		std::string title;
		unsigned int width;
		unsigned int height;
		bool vsync;
		//EventCallbackFn callback;

		Data(std::string_view t = "Bridge Maker App", unsigned int w = 1280, unsigned int h = 720, bool vs = false) : width(w), height(h), title(t), vsync(vs) {}
	};

	static std::unique_ptr<AbstractWindow> create(const Data& props = Data());

};


BM_END