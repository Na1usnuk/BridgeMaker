export module bm.gfx.scene;

import std;

import bm.gfx.obj;

namespace bm::gfx
{

	export class Scene
	{
	public:

		Scene() = default;
		~Scene() = default;

		void add(Object::KPtrRef mesh)
		{
			m_objects.push_back(mesh);
		}

		const std::vector<ObjectPtr>& getObjects() const { return m_objects; }

	private:

		std::vector<ObjectPtr> m_objects;

	};


}