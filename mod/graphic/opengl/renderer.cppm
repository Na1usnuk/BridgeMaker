export module bm.gfx:renderer;

import std;

import :array;
import :shader;
import :mesh;
import :material;
import :texture;
import :scene;

import bm.core;

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
	
			bool blend = false;
			bool depth_test;
			DepthFunc depth_test_func;
			BlendFunc blend_func;
			int texture_slot_count;
			std::vector<int> bound_textures;
		};
	
	public:
	
		Renderer();
		~Renderer();
	
		void clear();
		void draw(Traits<VertexArray>::KPtrRef vao, Traits<Shader>::KSPtrRef shader, Mesh::DrawAs draw_as);
		void draw(Traits<Scene>::PtrRef scene, Traits<Camera>::KPtrRef camera) { draw(*scene, *camera); }
		void draw(Scene& scene, Camera& camera);
	
		void setPolygonMode(PolygonMode mode = PolygonMode::Fill);
		void setDepthTesting(bool value);
		void setDepthTestFunc(DepthFunc func = DepthFunc::LESS);
		void setDepthWrite(bool b);
		void setBlend(bool value);
		void setBlendFunc(BlendSrc src = BlendSrc::ALPHA, BlendFunc func = BlendFunc::ONE_MINUS_SRC_ALPHA);
	
		void setView(const std::array<int, 4>& viewport);
		void setBackgroundColor(const RGBA_t& rgba);
		void setBackgroundColor(const RGB_t& rgb);
		void clearColor(const RGBA_t& rgba);
		void clearColor(const RGB_t& rgb);
	
		const std::array<int, 4>& getView() const { return m_state_cache.viewport; }
		int getTextureSlotCount() const { return m_state_cache.texture_slot_count; }
	
	
	private:
	
		StateCache m_state_cache;
	
	};


	struct QuadVertex
	{
		glm::vec3 position;
		glm::vec2 texcoord;
		glm::vec4 color;
		float texslot;
	};

	export class Quad
	{
	public:

		Quad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color = glm::vec4(1.f), Traits<Texture>::KSPtrRef texture = nullptr);
		Quad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color = glm::vec4(1.f), Traits<Texture>::KSPtrRef texture = nullptr);

		Traits<Texture>::KSPtrRef getTexture() const { return m_tex; }
		void setTexture(Traits<Texture>::KSPtrRef texture) { m_tex = texture; }

		const glm::vec3& getPosition() const { return m_pos; }
		void setPosition(const glm::vec3& position) { m_pos = position; }
		void setPosition(const glm::vec2& position) { setPosition(glm::vec3(position.x, position.y, 1.f)); }

		const glm::vec2& getSize() const { return m_size; }
		void setSize(const glm::vec2& size) { m_size = size; }

		const glm::vec4& getColor() const { return m_color; }
		void setColor(const glm::vec4& color) { m_color = color; }
		void setColor(const glm::vec3& color) { setColor(glm::vec4(color.x, color.y, color.z, 1.f)); }

	private:

		glm::vec3 m_pos;
		glm::vec2 m_size;
		glm::vec4 m_color;
		Traits<Texture>::SPtr m_tex;

	};


	// Can only draw quads
	export class ScreenRenderer
	{
	private:

		struct Data
		{
			Data();

			static constexpr std::uint32_t max_quads = 10;
			static constexpr std::uint32_t max_vertices = max_quads * 4;
			static constexpr std::uint32_t max_indices = max_quads * 6;
			static constexpr std::uint16_t max_texture_slots = 8;

			Traits<VertexArray>::Ptr vao = nullptr;
			Traits<Shader>::SPtr shader = nullptr;

			std::size_t quad_count = 0;

			std::array<QuadVertex, max_vertices> vertices;

			std::array<Traits<Texture>::SPtr, max_texture_slots> texture_slots;
			int texture_slot_index = 1; // 0 = white texture
		};

	public:

		ScreenRenderer()
		{
		}
		~ScreenRenderer() = default;

		void submit(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color = glm::vec4(1.f), Traits<Texture>::KSPtrRef texture = nullptr);
		void submit(const glm::vec3& position, const glm::vec2& size, Traits<Texture>::KSPtrRef texture = nullptr, const glm::vec4& color = glm::vec4(1.f))
		{
			submit(position, size, color, texture);
		}
		// Position in vec2 overload
		void submit(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color = glm::vec4(1.f), Traits<Texture>::KSPtrRef texture = nullptr)
		{
			submit({ position.x, position.y, 0.f }, size, color, texture);
		}
		void submit(const glm::vec2& position, const glm::vec2& size, Traits<Texture>::KSPtrRef texture = nullptr, const glm::vec4& color = glm::vec4(1.f))
		{
			submit(position, size, color, texture);
		}

		void draw();

		void setCamera(Traits<ScreenCamera>::KPtrRef camera) { m_camera = camera; }

	private:

		Data m_data;

		Traits<ScreenCamera>::OPtr m_camera = nullptr;
	};

}
