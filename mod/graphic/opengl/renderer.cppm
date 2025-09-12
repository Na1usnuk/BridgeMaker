export module bm.gfx.renderer;

import std;

import bm.window;

import bm.gfx.buffer.index;
import bm.gfx.vertexarray;
import bm.gfx.shader;
import bm.gfx.mesh;
import bm.gfx.texture;

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
	void draw(const std::shared_ptr<VertexArray>& vao, const std::shared_ptr<IndexBuffer>& ibo, const std::shared_ptr<Shader>& shader) { draw(*vao, *ibo, *shader); }
	void draw(const Mesh& mesh) { draw(mesh.getVertexArray(), mesh.getIndexBuffer(), mesh.getShader()); }
	void draw(const std::shared_ptr<Mesh>& mesh) { draw(*mesh); }
	
	void setPolygonMode(PolygonMode mode = PolygonMode::Fill);
	void setView(const std::array<int, 4>& viewport);
	void setBackgroundColor(const RGBA_t& rgba);
	void setBackgroundColor(const RGB_t& rgb);
	void clearColor(const RGBA_t& rgba);
	void clearColor(const RGB_t& rgb);


private:

	StateCache m_state_cache;

};

}
