export module bm.gfx:renderer;

import std;

import :utility;

import :buffer;

import :camera;
import :scene;

namespace bm::gfx
{

	class VertexArray
	{
	public:

		VertexArray();
		~VertexArray();

		VertexArray(const VertexArray&) = delete;
		VertexArray& operator=(const VertexArray&) = delete;

		VertexArray(VertexArray&& oth) noexcept;
		VertexArray& operator=(VertexArray&& oth) noexcept;

		void bind() const;
		static void unbind();
		unsigned int getId() const { return m_id; }

		void addVertexBuffer(const VertexBuffer& vbo, const VertexLayout& layout);
		void setIndexBuffer(const IndexBuffer& ibo);

	private:

		void init();
		void destroy();

	private:

		unsigned int m_id = 0;
		std::size_t m_attrib_count = 0;

	};


	export class Renderer
	{
	private:

		// Shape of object
		struct Shape
		{
			VertexArray vao;
			VertexBuffer vbo;
			std::optional<IndexBuffer> ibo;

			int mode;
		};

		// Helpers to map shaders to program
		struct ShaderProgramKey
		{
			std::array<Handler<ShaderSource>, 2> sources; 
			bool operator==(const ShaderProgramKey& other) const { return sources == other.sources; }
		};
		struct ShaderProgramKeyHash
		{
			std::size_t operator()(const ShaderProgramKey& key) const noexcept;
		};

		template<class K, class V>
		using HandlerMap = std::unordered_map<Handler<K>, V, HandlerHash>;

		struct Data
		{
			HandlerMap<Mesh, Shape> shapes;
			HandlerMap<ShaderSource, Shader> shaders;
			std::unordered_map<ShaderProgramKey, ShaderProgram, ShaderProgramKeyHash, std::equal_to<ShaderProgramKey>> programs;
		};

	public:
	
		Renderer();
		~Renderer();
	
		void clearColor(std::array<float, 4> color);
		void setViewportSize(int width, int height);

		// Slow operation that allocate GPU resources. 
		void prepare(const Scene& scene);
		// Main draw command. Use prepare, or first frame will be extremly slow
		void draw(const Scene& scene, Camera& camera);
		// Deallocate all GPU resources.
		void clear();

	private:

		void makeShape(Handler<Mesh> handler);
		void makeShader(Handler<ShaderSource> handler, Shader::Stage stage);
		void makeShaderProgram(Handler<ShaderSource> vertex, Handler<ShaderSource> fragment);
	
	private:
	
		Data m_data;
	
	};

}
	/*

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

			std::array<unsigned int, max_indices> makeIndices();

			VertexArray vao;
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

		void setCamera(ScreenCamera& camera) { m_camera = &camera; }

	private:

		Data m_data;

		Traits<ScreenCamera>::OPtr m_camera = nullptr;
	};

}
*/