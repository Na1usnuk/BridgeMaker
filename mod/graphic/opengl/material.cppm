module;

#include "glm/vec4.hpp"

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

		void setTexture(Traits<Texture>::KSPtrRef texture) { m_texture = texture; }
		Traits<Texture>::KSPtrRef getTexture() const { return m_texture; }

		Traits<Shader>::KSPtrRef getShader() const { return m_shader; }

		template<typename... Args>
		static Traits<Material>::SPtr make(Args&&... args) { return std::make_shared<Material>(std::forward<Args>(args)...); }

	private:

		Traits<Shader>::SPtr m_shader;
		Traits<Texture>::SPtr m_texture;
		glm::vec4 m_color = {1.f, 1.f, 1.f, 1.f};

	};

	export using MaterialPtr = Traits<Material>::Ptr;

}