export module bm.gfx:manager;

import std;

import bm.core;

import :object;
import :material;
import :mesh;
import :source;
import :image;

import :utility;

namespace bm::gfx
{

	export struct ResourceManager
	{
		HandleStorage<Object> objects;
		HandleStorage<Material> materials;
		HandleStorage<Mesh> meshes;
		HandleStorage<Image> images;
		HandleStorage<ShaderSource> shader_sources;
	};

}