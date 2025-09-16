export module bm.gfx.scene;

import std;

import bm.gfx.obj;

namespace bm::gfx
{

	export class Scene
	{
	public:

		using Ptr = std::shared_ptr<Scene>;
		using KPtrRef = const Ptr&;

	public:

		Scene() = default;
		~Scene() = default;

		void add(Object::KPtrRef mesh)
		{
			m_objects.push_back(mesh);
		}

		const std::vector<ObjectPtr>& getObjects() const { return m_objects; }

		static Ptr make() { return std::make_shared<Scene>(); }

	private:

		std::vector<ObjectPtr> m_objects;

	};


}