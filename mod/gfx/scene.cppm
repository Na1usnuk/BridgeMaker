export module bm.gfx:scene;

import std;

import :object;

import bm.core;

namespace bm::gfx
{

	export class Scene
	{
	public:

		Scene(std::string_view name = "Scene") : m_name(name) {}
		virtual ~Scene() = default;

		Scene(const Scene&) = delete;
		Scene& operator=(const Scene&) = delete;

		Scene(Scene&&) noexcept = default;
		Scene& operator=(Scene&&) noexcept = default;

		const std::string& getName() const { return m_name; }
		void setName(std::string_view name) { m_name = name; }

		// Optional overrides
		virtual void onUpdate(float deltaTime) {}
		virtual void onRender() {}
		virtual void onStartup() = 0;
		virtual void onShutdown() = 0;

		void addObject(core::Handle<Object> handler) noexcept
		{
			m_objects.push_back(handler);
		}

		void addLight(core::Handle<Light> handler) noexcept
		{
			m_lights.push_back(handler);
		}

		std::span<const core::Handle<Object>> getObjects() const noexcept
		{ 
			return { m_objects };
		}

		std::span<const core::Handle<Light>> getLights() const noexcept
		{ 
			return { m_lights };
		}

	private:

		std::string m_name;
		std::vector<core::Handle<Object>> m_objects;
		std::vector<core::Handle<Light>> m_lights;

	};

}