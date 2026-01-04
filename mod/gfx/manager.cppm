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

	export class ResourceManager
	{
	private:

		template<class Asset>
		struct Slot
		{
			std::optional<Asset> asset;
			std::uint32_t generation = 0;
		};

		template<class Asset>
		struct Container
		{
			std::vector<Slot<Asset>> container;
			std::vector<std::uint32_t> free_slots;
		};

		template<class Asset>
		using Optional = std::optional<std::reference_wrapper<Asset>>;

		template<class Asset>
		using Handler = Handler<Asset>;

	public:

		static Handler<Image>			load(Image&& material);
		static Handler<Material>		load(Material&& material);
		static Handler<ShaderSource>	load(ShaderSource&& source);
		static Handler<Mesh>			load(Mesh&& mesh);
		static Handler<VertexLayout>	load(VertexLayout&& mesh);
		static Handler<Object>			load(Object&& object);

		static void unload(Handler<ShaderSource> handler);
		static void unload(Handler<Image> handler);
		static void unload(Handler<Mesh> handler);
		static void unload(Handler<VertexLayout> handler);
		static void unload(Handler<Material> handler);
		static void unload(Handler<Object> handler);

		static Optional<ShaderSource>	get(Handler<ShaderSource> handler);
		static Optional<Image>			get(Handler<Image> handler);
		static Optional<Mesh>			get(Handler<Mesh> handler);
		static Optional<VertexLayout>	get(Handler<VertexLayout> handler);
		static Optional<Material>		get(Handler<Material> handler);
		static Optional<Object>			get(Handler<Object> handler);

	private:

		template<class Asset>
		static std::uint32_t next(Container<Asset>& container);
		template<class Asset, typename... Args>
		static Handler<Asset> load(Container<Asset>& container, Args&&... args);
		template<class Asset>
		static void unload(Container<Asset>& container, Handler<Asset> handler);
		template<class Asset>
		static Optional<Asset> get(Container<Asset>& container, Handler<Asset> handler);

	private:

		static Container<ShaderSource> m_sources;
		static Container<Mesh> m_meshes;
		static Container<VertexLayout> m_layouts;
		static Container<Material> m_materials;
		static Container<Object> m_objects;
		static Container<Image> m_images;

	};

}