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
		Material(Handler<ShaderSource> vertex_shader_source, Handler<ShaderSource> fragment_shader_source) :
			m_vertex(vertex_shader_source),
			m_fragment(fragment_shader_source)
		{}

		Handler<ShaderSource> getVertexShaderSource() const noexcept { return m_vertex; }
		Handler<ShaderSource> getFragmentShaderSource() const noexcept { return m_fragment; }

		void setVertexShaderSource(Handler<ShaderSource> handler) noexcept { m_vertex = handler; }
		void setFragmentShaderSource(Handler<ShaderSource> handler) noexcept { m_fragment = handler; }

		void setDiffuseImage(Handler<Image> diffuse) noexcept { m_diffuse = diffuse; }
		void setNormalImage(Handler<Image> normal) noexcept { m_normal = normal; }

	public:

		Description description;

	private:

		Handler<ShaderSource> m_vertex;
		Handler<ShaderSource> m_fragment;

		Handler<Image> m_diffuse;
		Handler<Image> m_normal;

	};

}