export module bm.gfx.scene;

import std;

import bm.gfx.obj;
import bm.gfx.camera;
import bm.event;
import bm.gfx.light;
import bm.traits;

namespace bm::gfx
{

	export class Scene
	{
	public:

		using Ptr = std::shared_ptr<Scene>;
		using KPtrRef = const Ptr&;

	public:

		Scene(std::string_view name = "") : m_name(name) {}
		virtual ~Scene() = default;

		// Optional overrides
		virtual void onUpdate(float deltaTime) {}
		virtual void onRender() {}
		virtual void onImGuiRender() {}
		virtual void onEvent(const Event& event) {}

		void addObject(Traits<Object>::KPtrRef obj)
		{
			m_objects.push_back(obj);
		}

		void removeObject(Traits<Object>::KPtrRef obj)
		{
			auto it = std::find(m_objects.begin(), m_objects.end(), obj);
			if (it != m_objects.end())
				m_objects.erase(it);
		}

		void addLight(Traits<Light>::KPtrRef light)
		{
			m_lights.push_back(light);
		}

		void removeLight(Traits<Light>::KPtrRef light)
		{
			auto it = std::find(m_lights.begin(), m_lights.end(), light);
			if (it != m_lights.end())
				m_lights.erase(it);
		}

		const std::vector<ObjectPtr>& getObjects() const { return m_objects; }
		std::vector<ObjectPtr>& getObjects() { return m_objects; }

		const std::vector<LightPtr>& getLights() const { return m_lights; }
		std::vector<LightPtr>& getLights() { return m_lights; }

		const std::string& getName() const { return m_name; }

		static Ptr make() { return std::make_shared<Scene>(); }

	private:

		std::string m_name;
		std::vector<ObjectPtr> m_objects;
		std::vector<LightPtr> m_lights;
		Traits<Camera>::Ptr m_camera;

	};

	export using ScenePtr = Scene::Ptr;

}