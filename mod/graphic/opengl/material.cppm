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

		using Ptr = Traits<Material>::Ptr;
		using KPtrRef = Traits<Material>::KPtrRef;
		using Action = std::function<void()>;

	public:

		Material(Traits<Shader>::KPtrRef shader) : m_shader(shader) { m_texture = AssetManager::get().load<Texture>("nothing_texture"); }


		void bind()
		{
			m_shader->bind();
			while (not m_action_queue.empty())
			{
				m_action_queue.front()(); // Calls Action
				m_action_queue.pop();
			}
		}

		template<typename... Args>
		void setUniform(std::string_view name, Args&&... args) 
		{ 
			m_action_queue.push([shader = m_shader, name = std::string(name), ... args = std::forward<Args>(args)]() mutable {
				shader->setUniform(name, std::forward<Args>(args)...);
				});
		}

		void setColor(const glm::vec4& color) { m_color = color; }
		const glm::vec4& getColor() const { return m_color; }

		void setTexture(Traits<Texture>::KPtrRef texture) { m_texture = texture; }
		Traits<Texture>::KPtrRef getTexture() const { return m_texture; }

		Traits<Shader>::KPtrRef getShader() const { return m_shader; }

		static Ptr make(ShaderPtr shader) { return std::make_shared<Material>(shader); }

	private:

		ShaderPtr m_shader;

		TexturePtr m_texture;
		glm::vec4 m_color = {1.f, 1.f, 1.f, 1.f};

		std::queue<Action> m_action_queue;

	};

}