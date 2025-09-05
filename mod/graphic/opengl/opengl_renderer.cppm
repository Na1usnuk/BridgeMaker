export module bm.gfx.renderer;

import std;

import bm.window;

import bm.gfx.buffer.index;
import bm.gfx.vertexarray;
import bm.gfx.shader;

namespace bm::gfx
{

export class Renderer
{
public:

	enum class PolygonMode : unsigned int
	{
		//magic numbers from GL
		Fill = 0x1B02,
		Line = 0x1B01,
		Point = 0x1B00
	};

	using RGB_t      = std::array<float, 3>;
	using RGBA_t     = std::array<float, 4>;

private:

	struct StateCache
	{
		RGBA_t clear_color;
		PolygonMode polygon_mode;
		std::array<int, 4> viewport;
	};

public:

	Renderer();
	~Renderer();

	void clear();
	void draw(const VertexArray&, const IndexBuffer&, const Shader&);
	
	void setPolygonMode(PolygonMode mode = PolygonMode::Fill);
	void setView(std::array<int, 4> viewport);
	void setBackgroundColor(RGBA_t rgba = {0.f, 0.f, 0.f, 1.f});
	void setBackgroundColor(RGB_t rgb = {0.f, 0.f, 0.f});
	void clearColor(RGBA_t rgba = { 0.f, 0.f, 0.f, 1.f });
	void clearColor(RGB_t rgb = { 0.f, 0.f, 0.f });


private:

	StateCache m_state_cache;

};

}
