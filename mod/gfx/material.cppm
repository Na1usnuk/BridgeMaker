export module bm.gfx:material;

import :source;
import :image;
import :uniform;
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
			std::uint64_t vertex_source = 1;
			std::uint64_t fragment_source = 1;
			
			std::uint64_t images = 1;
			std::uint64_t uniforms = 1;

			constexpr bool operator==(const Version& oth) const noexcept = default;
		};

	private:

		//struct ImageDesc
		//{
		//	std::string name; 
		//	core::Handle<Image> image; 
		//};

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

		const Version& getVersion() const noexcept { return m_version; }

	public:

		Bindings bindings;

	private:

		core::Handle<ShaderSource> m_vertex;
		core::Handle<ShaderSource> m_fragment;

		// Other types of shader will be added as optionals
		//std::optional<core::Handle<ShaderSource>> m_compute;
		//std::optional<core::Handle<ShaderSource>> m_geometry;
		
		Version m_version;
	};

}