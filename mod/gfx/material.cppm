export module bm.gfx:material;

import :source;
import :image;
import :utility;

import glm;

import bm.core;

import std;

namespace bm::gfx
{

	export class Material
	{
	public:

		struct Description
		{
			glm::vec4 color{ 1.0f, 1.0f, 1.0f, 1.0f };
			glm::vec3 ambient{ 0.2f, 0.2f, 0.2f };
			glm::vec3 diffuse{ 1.0f, 1.0f, 1.0f };
			glm::vec3 specular{ 0.5f, 0.5f, 0.5f };
			float shininess{ 32.0f };
		};

	public:

		Material() = default;
		Material(core::Handle<ShaderSource> vertex_shader_source, core::Handle<ShaderSource> fragment_shader_source) :
			m_vertex(vertex_shader_source),
			m_fragment(fragment_shader_source)
		{}

		core::Handle<ShaderSource> getVertexShaderSource() const noexcept { return m_vertex; }
		core::Handle<ShaderSource> getFragmentShaderSource() const noexcept { return m_fragment; }

		void setVertexShaderSource(core::Handle<ShaderSource> handler) noexcept { m_vertex = handler; }
		void setFragmentShaderSource(core::Handle<ShaderSource> handler) noexcept { m_fragment = handler; }

		void setDiffuseImage(core::Handle<Image> diffuse) noexcept { m_diffuse = diffuse; }
		void setNormalImage(core::Handle<Image> normal) noexcept { m_normal = normal; }

		core::Handle<Image> getDiffuseImage() const noexcept { return m_diffuse; }
		core::Handle<Image> getNormalImage() const noexcept { return m_normal; }

	public:

		Description description;

	private:

		core::Handle<ShaderSource> m_vertex;
		core::Handle<ShaderSource> m_fragment;

		core::Handle<Image> m_diffuse;
		core::Handle<Image> m_normal;

	};

}