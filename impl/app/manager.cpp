module bm.app:manager;
import :manager;

import std;
import bm.core;
import bm.gfx;

namespace bm
{
	ResourceManager::Container<gfx::Texture> ResourceManager::m_textures{};
	ResourceManager::Container<gfx::ShaderSource> ResourceManager::m_sources{};
	ResourceManager::Container<gfx::Mesh> ResourceManager::m_meshes{};
	ResourceManager::Container<gfx::Material> ResourceManager::m_materials{};


	ResourceManager::Handler<gfx::Texture> ResourceManager::load(gfx::Texture&& texture)
	{
		auto handler = load<gfx::Texture>(m_textures, std::move(texture));
		core::log::trace("Texture {} generation {} LOADED", handler.index, handler.generation);
		return handler;
	}

	ResourceManager::Handler<gfx::ShaderSource> ResourceManager::load(gfx::ShaderSource&& source)
	{
		auto handler = load<gfx::ShaderSource>(m_sources, std::move(source));
		core::log::trace("ShaderSource {} generation {} LOADED", handler.index, handler.generation);
		return handler;
	}

	ResourceManager::Handler<gfx::Mesh> ResourceManager::load(gfx::Mesh&& mesh)
	{
		auto handler = load<gfx::Mesh>(m_meshes, std::move(mesh));
		core::log::trace("Mesh {} generation {} LOADED", handler.index, handler.generation);
		return handler;
	}

	ResourceManager::Handler<gfx::Material> ResourceManager::load(gfx::Material&& material)
	{
		auto handler = load<gfx::Material>(m_materials, std::move(material));
		core::log::trace("Material {} generation {} LOADED", handler.index, handler.generation);
		return handler;
	}


	void ResourceManager::unload(Handler<gfx::Texture> handler)
	{
		core::log::trace("Texture {} generation {} UNLOADED", handler.index, handler.generation);
		unload<gfx::Texture>(m_textures, handler);
	}

	void ResourceManager::unload(Handler<gfx::ShaderSource> handler)
	{
		core::log::trace("ShaderSource {} generation {} UNLOADED", handler.index, handler.generation);
		unload<gfx::ShaderSource>(m_sources, handler);
	}

	void ResourceManager::unload(Handler<gfx::Mesh> handler)
	{
		core::log::trace("Mesh {} generation {} UNLOADED", handler.index, handler.generation);
		unload<gfx::Mesh>(m_meshes, handler);
	}

	void ResourceManager::unload(Handler<gfx::Material> handler)
	{
		core::log::trace("Material {} generation {} UNLOADED", handler.index, handler.generation);
		unload<gfx::Material>(m_materials, handler);
	}


	std::optional<std::reference_wrapper<gfx::Texture>> ResourceManager::get(ResourceManager::Handler<gfx::Texture> handler)
	{
		return get<gfx::Texture>(m_textures, handler);
	}

	std::optional<std::reference_wrapper<gfx::ShaderSource>> ResourceManager::get(ResourceManager::Handler<gfx::ShaderSource> handler)
	{
		return get<gfx::ShaderSource>(m_sources, handler);
	}

	std::optional<std::reference_wrapper<gfx::Mesh>> ResourceManager::get(ResourceManager::Handler<gfx::Mesh> handler)
	{
		return get<gfx::Mesh>(m_meshes, handler);
	}

	std::optional<std::reference_wrapper<gfx::Material>> ResourceManager::get(ResourceManager::Handler<gfx::Material> handler)
	{
		return get<gfx::Material>(m_materials, handler);
	}


	template<class Asset>
	std::uint32_t ResourceManager::next(ResourceManager::Container<Asset>& container)
	{
		if (not container.free_slots.empty())
		{
			auto slot = container.free_slots.back();
			container.free_slots.pop_back();
			return slot;
		}
		return container.container.size();
	}

	template<class Asset, typename... Args>
	ResourceManager::Handler<Asset> ResourceManager::load(ResourceManager::Container<Asset>& container, Args&&... args)
	{
		Handler<Asset> handler;
		auto asset = std::make_unique<Asset>(std::forward<Args>(args)...);
		const auto slot = next<Asset>(container);

		if (slot == container.container.size())
		{
			container.container.emplace_back(std::move(asset), 0);
			handler.generation = 0;
		}
		else
		{
			container.container[slot].asset = std::move(asset);
			handler.generation = ++container.container[slot].generation;
		}

		handler.index = slot;

		return handler;
	}

	template<class Asset>
	void ResourceManager::unload(ResourceManager::Container<Asset>& container, ResourceManager::Handler<Asset> handler)
	{
		core::verify(container.container.size() > handler.index, "ResourceManager unload out of range asset");

		if (container.container[handler.index].generation != handler.generation)
		{
			core::log::warning("ResourceManager::unload ignored: stale handler {}", handler.index);
			return;
		}

		container.container[handler.index].asset.reset();
		container.container[handler.index].generation++;
		container.free_slots.push_back(handler.index);
	}

	template<class Asset>
	std::optional<std::reference_wrapper<Asset>> ResourceManager::get(ResourceManager::Container<Asset>& container, ResourceManager::Handler<Asset> handler)
	{
		if (handler.index >= container.container.size())
			return std::nullopt;

		if (container.container[handler.index].generation not_eq handler.generation)
			return std::nullopt;

		if (not container.container[handler.index].asset)
			return std::nullopt;

		return std::make_optional<std::reference_wrapper<Asset>>(std::ref(*container.container[handler.index].asset));
	}
}