export module bm.gfx.material;


import bm.gfx.shader;
import bm.gfx.texture;

import std;

namespace bm::gfx
{

	export class Material
	{
	public:

		using Ptr = std::shared_ptr<Material>;
		using KPtrRef = const Ptr&;
		using Action = std::function<void()>;

		using RGBA = std::array<float, 4>;

	public:

		Material(ShaderPtr shader);


		void bind()
		{
			m_shader->bind();
			while (not m_action_queue.empty())
			{
				m_action_queue.front()();
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

		void setColor(std::array<float, 4> color)
		{
			setUniform("u_color", color[0], color[1], color[2], color[3]);
		}

		void setTexture(TexturePtr texture)
		{
			m_texture = texture;
		}

		static Ptr make(ShaderPtr shader) { return std::make_shared<Material>(shader); }

	private:

		ShaderPtr m_shader;

		TexturePtr m_texture;
		RGBA m_color = {1.f, 1.f, 1.f, 1.f};

		std::queue<Action> m_action_queue;

	};

}