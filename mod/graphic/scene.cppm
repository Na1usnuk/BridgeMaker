export module bm.gfx.scene;

import std;

import bm.gfx.object;
import bm.gfx.camera;
import bm.event;
import bm.gfx.light;
import bm.traits;

namespace bm::gfx
{

	export class Scene
	{
	public:

		Scene(std::string_view name = "") : m_name(name) {}
		virtual ~Scene() = default;

		// Optional overrides
		virtual void onUpdate(float deltaTime) {}
		virtual void onRender() {}
		virtual void onImGuiRender() {}
		virtual void onEvent(const Event& event) {}

		void addObject(Traits<Object>::Ptr obj)
		{
			m_objects.push_back(std::move(obj));
		}

		//void removeObject(Traits<Object>::Ptr&& obj)
		//{
		//	auto it = std::find(m_objects.begin(), m_objects.end(), obj);
		//	if (it != m_objects.end())
		//		m_objects.erase(it);
		//}

		void addLight(Traits<Light>::Ptr light)
		{
			m_lights.push_back(std::move(light));
		}

		//void removeLight(Traits<Light>::KPtrRef light)
		//{
		//	auto it = std::find(m_lights.begin(), m_lights.end(), light);
		//	if (it != m_lights.end())
		//		m_lights.erase(it);
		//}

		const std::vector<Traits<Object>::Ptr>& getObjects() const { return m_objects; }
		std::vector<Traits<Object>::Ptr>& getObjects() { return m_objects; }

		const std::vector<Traits<Light>::Ptr>& getLights() const { return m_lights; }
		std::vector<Traits<Light>::Ptr>& getLights() { return m_lights; }

		const std::string& getName() const { return m_name; }

		template<typename S, typename... Args>
			requires std::is_base_of_v<Scene, S>
		static Traits<S>::Ptr make(Args&&... args) { return std::make_unique<S>(std::forward<Args>(args)...); }

		static Traits<Scene>::Ptr make() { return std::make_unique<Scene>(); }

	private:

		std::string m_name;
		std::vector<Traits<Object>::Ptr> m_objects;
		std::vector<Traits<Light>::Ptr> m_lights;

	};

}