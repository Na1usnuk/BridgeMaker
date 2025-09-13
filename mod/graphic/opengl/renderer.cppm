export module bm.gfx.renderer;

import std;

import bm.window;

import bm.gfx.buffer.index;
import bm.gfx.vertexarray;
import bm.gfx.shader;
import bm.gfx.mesh;
import bm.gfx.texture;
import bm.gfx.scene;
import bm.gfx.camera;

namespace bm::gfx
{

export class Renderer
{
public:

	enum class PolygonMode
	{
		//magic numbers from GL
		Fill = 0x1B02,
		Line = 0x1B01,
		Point = 0x1B00
	};

	enum class DepthFunc
	{
		//magic numbers from GL
		NEVER    = 0x0200,
		LESS     = 0x0201, 
		EQUAL    = 0x0202, 
		LEQUAL   = 0x0203, 
		GREATER  = 0x0204,
		NOTEQUAL = 0x0205, 
		GEQUAL   = 0x0206,
		ALWAYS   = 0x0207,
	};

	enum class BlendFunc 
	{
		ZERO                     = 0x0000,
		ONE                      = 0x0001, 
		ONE_MINUS_SRC_COLOR      = 0x0301, 
		ONE_MINUS_SRC_ALPHA      = 0x0303, 
		DST_ALPHA                = 0x0304, 
		ONE_MINUS_DST_ALPHA      = 0x0305, 
		DST_COLOR                = 0x0306,
		ONE_MINUS_DST_COLOR      = 0x0307, 
		CONSTANT_COLOR           = 0x8001, 
		ONE_MINUS_CONSTANT_COLOR = 0x8002, 
		CONSTANT_ALPHA           = 0x8003, 
		ONE_MINUS_CONSTANT_ALPHA = 0x8004
	};

	enum class BlendSrc
	{
		COLOR = 0x0300,
		ALPHA = 0x0302,
	};

	using RGB_t      = std::array<float, 3>;
	using RGBA_t     = std::array<float, 4>;

private:

	struct StateCache
	{
		RGBA_t clear_color;
		PolygonMode polygon_mode;
		std::array<int, 4> viewport;

		bool blend;
		bool depth_test;
		DepthFunc depth_test_func;
		BlendFunc blend_func;
	};

public:

	Renderer();
	~Renderer();

	void clear();
	void draw(const VertexArray&, const IndexBuffer&, const Shader&);
	void draw(const std::shared_ptr<VertexArray>& vao, const std::shared_ptr<IndexBuffer>& ibo, const std::shared_ptr<Shader>& shader) { draw(*vao, *ibo, *shader); }
	void draw(const Mesh& mesh) { draw(mesh.getVertexArray(), mesh.getIndexBuffer(), mesh.getShader()); }
	void draw(const std::shared_ptr<Mesh>& mesh) { draw(*mesh); }
	void draw(const Scene& scene, const Camera& camera)
	{
		for (const auto& obj : scene.getObjects())
		{
			auto shader = obj->getShader();
			shader->setUniform("u_model", obj->getModel());
			shader->setUniform("u_view", camera.getView());
			shader->setUniform("u_projection", camera.getProjection());
			draw(obj);
		}
	}
	
	void setPolygonMode(PolygonMode mode = PolygonMode::Fill);

	void setDepthTesting(bool value);
	void setDepthTestFunc(DepthFunc func = DepthFunc::LESS);

	void setBlend(bool value);
	void setBlendFunc(BlendSrc src = BlendSrc::ALPHA, BlendFunc func = BlendFunc::ONE_MINUS_SRC_ALPHA);

	void setView(const std::array<int, 4>& viewport);
	const std::array<int, 4>& getView() const { return m_state_cache.viewport; }
	void setBackgroundColor(const RGBA_t& rgba);
	void setBackgroundColor(const RGB_t& rgb);
	void clearColor(const RGBA_t& rgba);
	void clearColor(const RGB_t& rgb);


private:

	StateCache m_state_cache;

};

}
