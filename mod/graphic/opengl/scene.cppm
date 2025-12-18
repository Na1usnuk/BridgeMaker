export module bm.gfx:scene;

import std;

import :object;

import bm.core;

namespace bm::gfx
{

	export class Scene
	{
	public:

		struct [[nodiscard]] ObjectHandler { std::size_t id; };
		struct [[nodiscard]] LightHandler { std::size_t id; };

	public:

		Scene(std::string_view name = "") : m_name(name) {}
		virtual ~Scene() = default;

		Scene(const Scene&) = delete;
		Scene& operator=(const Scene&) = delete;

		Scene(Scene&&) noexcept = default;
		Scene& operator=(Scene&&) noexcept = default;

		const std::string& getName() const { return m_name; }
		void setName(std::string_view name) { m_name = name; }

		// Optional overrides
		virtual void onUpdate(float deltaTime) {}
		virtual void onEvent(event::Event& event) {}
		virtual void onRender() {}

		[[nodiscard]]
		ObjectHandler addObject(Object&& obj)
		{
			m_objects.push_back(std::move(obj));
			return { m_objects.size() - 1 };
		}

		[[nodiscard]]
		LightHandler addLight(Light&& light)
		{
			m_lights.push_back(std::move(light));
			return { m_lights.size() - 1 };
		}

		const Object& getObject(ObjectHandler handler) const
		{ 
			core::verify(handler.id < m_objects.size(), "No object found");
			return m_objects[handler.id];
		}
		Object& getObject(ObjectHandler handler) { return const_cast<Object&>(static_cast<const Scene&>(*this).getObject(handler)); }

		const Light& getLight(LightHandler handler) const
		{ 
			core::verify(handler.id < m_lights.size(), "No light found");
			return m_lights[handler.id];
		}
		Light& getLight(LightHandler handler) { return const_cast<Light&>(static_cast<const Scene&>(*this).getLight(handler)); }

		const std::vector<Object>& getObjects() const { return m_objects; }
		std::vector<Object>& getObjects() { return m_objects; }

		const std::vector<Light>& getLights() const { return m_lights; }
		std::vector<Light>& getLights() { return m_lights; }

		template<typename S, typename... Args>
			requires std::is_base_of_v<Scene, S>
		static Traits<S>::Ptr make(Args&&... args) { return std::make_unique<S>(std::forward<Args>(args)...); }

		static Traits<Scene>::Ptr make() { return std::make_unique<Scene>(); }

	private:

		std::string m_name;
		std::vector<Object> m_objects;
		std::vector<Light> m_lights;

	};

}