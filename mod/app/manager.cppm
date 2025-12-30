export module bm.app:manager;

import std;

import bm.core;
import bm.gfx;

namespace bm
{

	export class ResourceManager
	{
	public:

		template<class Asset>
		struct [[nodiscard]] Handler { std::uint32_t index, generation; };

	private:

		template<class Asset>
		struct Slot
		{
			std::unique_ptr<Asset> asset;
			std::uint32_t generation = 0;
		};

		template<class Asset>
		struct Container
		{
			std::vector<Slot<Asset>> container;
			std::vector<std::uint32_t> free_slots;
		};

	public:

		static Handler<gfx::Texture> load(gfx::Texture&& texture);
		static Handler<gfx::ShaderSource> load(gfx::ShaderSource&& source);
		static Handler<gfx::Mesh> load(gfx::Mesh&& mesh);
		static Handler<gfx::Material> load(gfx::Material&& material);

		static void unload(Handler<gfx::Texture> handler);
		static void unload(Handler<gfx::ShaderSource> handler);
		static void unload(Handler<gfx::Mesh> handler);
		static void unload(Handler<gfx::Material> handler);

		static std::optional<std::reference_wrapper<gfx::Texture>> get(Handler<gfx::Texture> handler);
		static std::optional<std::reference_wrapper<gfx::ShaderSource>> get(Handler<gfx::ShaderSource> handler);
		static std::optional<std::reference_wrapper<gfx::Mesh>> get(Handler<gfx::Mesh> handler);
		static std::optional<std::reference_wrapper<gfx::Material>> get(Handler<gfx::Material> handler);

	private:

		// Next free slot
		template<class Asset>
		static std::uint32_t next(Container<Asset>& container);

		template<class Asset, typename... Args>
		static Handler<Asset> load(Container<Asset>& container, Args&&... args);

		template<class Asset>
		static void unload(Container<Asset>& container, Handler<Asset> handler);

		template<class Asset>
		static std::optional<std::reference_wrapper<Asset>> get(Container<Asset>& container, Handler<Asset> handler);

	private:

		static Container<gfx::Texture> m_textures;
		static Container<gfx::ShaderSource> m_sources;
		static Container<gfx::Mesh> m_meshes;
		static Container<gfx::Material> m_materials;

	};

}