module;

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

export module bm.gfx.screen_renderer;

import bm.traits;

import bm.gfx.shader;
import bm.gfx.buffer.vertex;
import bm.gfx.buffer.index;
import bm.gfx.vertexarray;
import bm.gfx.mesh;
import bm.gfx.texture;
import bm.gfx.material;
import bm.gfx.object;
import bm.assetmanager;

import bm.gfx.camera.screen;

import std;

namespace bm::gfx
{
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

		Quad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color = glm::vec4(1.f), Traits<Texture>::KSPtrRef texture = nullptr) :
			m_pos(position),
			m_size(size),
			m_color(color),
			m_tex(texture)
		{ 
			if (m_tex == nullptr)
				m_tex = AssetManager::get().loadTexture("nothing");
		}
		Quad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color = glm::vec4(1.f), Traits<Texture>::KSPtrRef texture = nullptr) :
			Quad(glm::vec3(position.x, position.y, 1.f), size, color, texture)
		{}

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
		{}
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