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
		Material(Handle<ShaderSource> vertex_shader_source, Handle<ShaderSource> fragment_shader_source) :
			m_vertex(vertex_shader_source),
			m_fragment(fragment_shader_source)
		{}

		Handle<ShaderSource> getVertexShaderSource() const noexcept { return m_vertex; }
		Handle<ShaderSource> getFragmentShaderSource() const noexcept { return m_fragment; }

		void setVertexShaderSource(Handle<ShaderSource> handler) noexcept { m_vertex = handler; }
		void setFragmentShaderSource(Handle<ShaderSource> handler) noexcept { m_fragment = handler; }

		void setDiffuseImage(Handle<Image> diffuse) noexcept { m_diffuse = diffuse; }
		void setNormalImage(Handle<Image> normal) noexcept { m_normal = normal; }

		Handle<Image> getDiffuseImage() const noexcept { return m_diffuse; }
		Handle<Image> getNormalImage() const noexcept { return m_normal; }

	public:

		Description description;

	private:

		Handle<ShaderSource> m_vertex;
		Handle<ShaderSource> m_fragment;

		Handle<Image> m_diffuse;
		Handle<Image> m_normal;

	};

}