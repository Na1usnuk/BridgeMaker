#pragma once

#include "core.hpp"

BM_START

template<typename Backend>
class Renderer
{
public:

	using Obj = typename Backend::Obj;
	using PolygonMode = typename Backend::PolygonMode;
	

public:

	Renderer() {}
	~Renderer() {}

	void draw(const Obj& object) const { m_impl.draw(object); }


	//void setBackgrounColor(float R, float G, float B, float A = 1.f) const { m_impl.setBackgroungColor(R, G, B, A); }
	void setPolygonMode(PolygonMode mode = PolygonMode::Fill) const { m_impl.setPolygonMode(mode); }

	void setView(int x, int y, int w, int h) const { BM_CORE_TRACE("setView to {0}x{1}", w, h); m_impl.setView(x, y, w, h); }

	void clear(float R, float G, float B, float A = 1.f) { m_impl.clearColor(R, G, B, A); }

private:

	Backend m_impl;

};

BM_END