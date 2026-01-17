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
	private:

		struct ImageDescription
		{
			core::Handle<Image> image; // Image of texture 
			std::string name; // Name of this texture in shader. 
		};

	public:

		//struct Description
		//{
		//	glm::vec4 color{ 1.0f, 1.0f, 1.0f, 1.0f };
		//	glm::vec3 ambient{ 0.2f, 0.2f, 0.2f };
		//	glm::vec3 diffuse{ 1.0f, 1.0f, 1.0f };
		//	glm::vec3 specular{ 0.5f, 0.5f, 0.5f };
		//	float shininess{ 32.0f };
		//};

	public:

		//Material() = default;
		//Material(core::Handle<ShaderSource> vertex_shader_source, core::Handle<ShaderSource> fragment_shader_source) :
		//	m_vertex(vertex_shader_source),
		//	m_fragment(fragment_shader_source)
		//{}


		void set(std::string_view name, core::Handle<Image> image)
		{
			//m_images.emplace_back(image, name);
		}

		void set(core::Handle<ShaderSource> shader_source)
		{
			m_sources.push_back(shader_source);
		}

		const std::vector<ImageDescription>& getImages() const noexcept
		{
			return m_images;
		}

	public:

		//Description description;

	private:

		std::vector<core::Handle<ShaderSource>> m_sources;
		std::vector<ImageDescription> m_images;

	};

}