export module bm.ui:debug;

export import imgui;

import bm.core;
import bm.platform;

import std;

namespace bm::ui
{
	export class ImGuiRenderer
	{
	public:

		ImGuiRenderer(const platform::Window& window);

		~ImGuiRenderer();

		template<class T>
			requires not std::is_invocable_v<T>
		void submit(T& t)
		{
			std::type_index id(typeid(T));
			if (not m_funcs.contains(id))
			{
				core::log::error("ImGuiRenderer submit object of not registered type {}", id.name());
				return;
			}
			auto call_func = [&t, this, id]()
				{
					m_funcs.at(id)(&t);
				};
			m_calls.push_back(call_func);
		}

		template<class F>
			requires std::is_invocable_v<F>
		void submit(F&& func)
		{
			m_calls.emplace_back(func);
		}

		template<typename T>
		void registerType(std::function<void(T&)> func)
		{
			std::type_index id(typeid(T));
			if (m_funcs.contains(id))
				core::log::warning("Replacement of ImGuiRenderer function for type {}", id.name());
			m_funcs[id] = [func](void* ptr) { func(*static_cast<T*>(ptr)); };
		}

		void draw();

	private:

		std::vector<std::function<void()>> m_calls;
		std::unordered_map<std::type_index, std::function<void(void*)>> m_funcs;

	};
}