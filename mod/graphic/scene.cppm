export module bm.gfx.scene;

import std;

import bm.gfx.mesh;

namespace bm::gfx
{

	export class Scene
	{
	public:

		Scene() = default;
		~Scene() = default;

		void add(Mesh::KPtrRef mesh)
		{
			m_meshes.push_back(mesh);
		}

		const std::vector<Mesh::Ptr>& getMeshes() const { return m_meshes; }

	private:

		std::vector<Mesh::Ptr> m_meshes;

	};


}