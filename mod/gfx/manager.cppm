export module bm.gfx:manager;

import std;

import bm.core;

import :object;
import :material;
import :mesh;
import :source;
import :image;
import :buffer;

namespace bm::gfx
{
	// Resource manager holding CPU assets
	export struct ResourceManager
	{
		core::HandleStorage<Object> objects;
		core::HandleStorage<Material> materials;
		core::HandleStorage<Mesh> meshes;
		core::HandleStorage<Image> images;
		core::HandleStorage<ShaderSource> shader_sources;
		core::HandleStorage<VertexLayout> vertex_layouts;

		void unload() noexcept
		{
			objects.unload();
			materials.unload();
			meshes.unload();
			images.unload();
			shader_sources.unload();
			vertex_layouts.unload();
		}
	};

}