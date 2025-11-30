module;

#include "glm/vec4.hpp"
#include "glm/vec3.hpp"
#include "glm/vec2.hpp"

export module bm.gfx.material;

import bm.gfx.shader;
import bm.gfx.texture;
import bm.assetmanager;

import bm.traits;

import std;

namespace bm::gfx
{

	export class Material
	{
	public:

		Material() : Material(AssetManager::get().loadShader("basic")) {}
		Material(Traits<Shader>::KSPtrRef shader) : m_shader(shader) { m_texture = AssetManager::get().loadTexture("nothing"); }
		Material(Traits<Shader>::KSPtrRef shader, Traits<Texture>::KSPtrRef texture) : m_shader(shader), m_texture(texture) {}
		~Material() = default;

		void bind()
		{
			m_shader->bind();
		}

		template<typename... Args>
		void setUniform(std::string_view name, Args&&... args)
		{
			m_shader->setUniform(name, std::forward<Args>(args)...);
		}

		void setColor(const glm::vec4& color) { m_color = color; }
		const glm::vec4& getColor() const { return m_color; }

		void setAmbient(const glm::vec3& amb) { m_amb = amb; }
		const glm::vec3& getAmbient() const { return m_amb; }

		void setDiffuse(const glm::vec3& dif) { m_dif = dif; }
		const glm::vec3& getDiffuse() const { return m_dif; }

		void setSpecular(const glm::vec3& spec) { m_spec = spec; }
		const glm::vec3& getSpecular() const { return m_spec; }

		void setShininess(float shininess) { m_shininess = shininess; }
		float getShininess() const { return m_shininess; }

		void setTexture(Traits<Texture>::KSPtrRef texture) { m_texture = texture; }
		Traits<Texture>::KSPtrRef getTexture() const { return m_texture; }

		void setShader(Traits<Shader>::KSPtrRef shader) { m_shader = shader; }
		Traits<Shader>::KSPtrRef getShader() const { return m_shader; }

		template<typename... Args>
		static Traits<Material>::SPtr make(Args&&... args) { return std::make_shared<Material>(std::forward<Args>(args)...); }

	private:

		Traits<Shader>::SPtr m_shader;
		Traits<Texture>::SPtr m_texture;
		glm::vec4 m_color{ 1.0f, 1.0f, 1.0f, 1.0f };
		glm::vec3 m_amb{ 0.2f, 0.2f, 0.2f };        
		glm::vec3 m_dif{ 1.0f, 1.0f, 1.0f };        
		glm::vec3 m_spec{ 0.5f, 0.5f, 0.5f };       
		float m_shininess{ 32.0f };                 


	};

}