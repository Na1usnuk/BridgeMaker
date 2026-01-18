export module bm.gfx:material;

import :source;
import :image;
import :utility;
import :image;

import glm;

import bm.core;

import std;

namespace bm::gfx
{

	export class Material
	{
	public:

		struct Version
		{
			std::uint64_t vertex_source = 0;
			std::uint64_t fragment_source = 0;
			
			std::uint64_t images = 0;
			std::uint64_t uniforms = 0;
		};

	private:

		using UniformValue = std::variant
			<
				float,
				int,
				glm::vec2,
				glm::vec3,
				glm::vec4,
				glm::mat3,
				glm::mat4
			>;

		struct UniformDesc
		{
			std::string name;
			UniformValue value;
		};

		struct ImageDesc
		{
			std::string name; 
			core::Handle<Image> image; 
		};

	public:

		Material() = default;
		Material(core::Handle<ShaderSource> vertex_shader_source, core::Handle<ShaderSource> fragment_shader_source) :
			m_vertex(vertex_shader_source),
			m_fragment(fragment_shader_source)
		{}

		void setVertexShaderSource(core::Handle<ShaderSource> vertex_source_handle) noexcept { m_vertex = vertex_source_handle; m_version.vertex_source++; }
		void setFragmentShaderSource(core::Handle<ShaderSource> fragment_source_handle) noexcept { m_fragment = fragment_source_handle; m_version.fragment_source++; }

		core::Handle<ShaderSource> getVertexShaderSource() const noexcept { return m_vertex; }
		core::Handle<ShaderSource> getFragmentShaderSource() const noexcept { return m_fragment; }

		// Not DRY
		template<typename T>
		void set(std::string name, const T& value)
		{
			auto it = std::ranges::find(m_uniforms, name, &UniformDesc::name);
			if (it not_eq m_uniforms.end())
				it->value = value;
			else
				m_uniforms.emplace_back({ std::move(name), {value} });
			m_version.uniforms++;
		}
		// Very not DRY
		void set(std::string name, core::Handle<Image> image)
		{
			auto it = std::ranges::find(m_images, name, &ImageDesc::name);
			if (it not_eq m_images.end())
				it->image = image;
			else
				m_images.emplace_back(std::move(name), image);
			m_version.images++;
		}

		std::span<const ImageDesc> getImages() const noexcept { return { m_images }; }
		std::span<const UniformDesc> getUniforms() const noexcept { return { m_uniforms }; }

		const Version& getVersion() const noexcept { return m_version; }

	private:

		core::Handle<ShaderSource> m_vertex;
		core::Handle<ShaderSource> m_fragment;

		// Other types of shader will be added as optionals
		//std::optional<core::Handle<ShaderSource>> m_compute;
		//std::optional<core::Handle<ShaderSource>> m_geometry;

		std::vector<ImageDesc> m_images;
		std::vector<UniformDesc> m_uniforms;
		
		Version m_version;
	};

}