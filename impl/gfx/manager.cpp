module bm.gfx:manager;

import :manager;

import :object;
import :material;
import :mesh;
import :source;
import :texture;
import :buffer;

import std;
import bm.core;

namespace bm::gfx
{
	ResourceManager::Container<ShaderSource> ResourceManager::m_sources{};
	ResourceManager::Container<Mesh> ResourceManager::m_meshes{};
	ResourceManager::Container<VertexLayout> ResourceManager::m_layouts{};
	ResourceManager::Container<Material> ResourceManager::m_materials{};
	ResourceManager::Container<Object> ResourceManager::m_objects{};
	ResourceManager::Container<Image> ResourceManager::m_images{};


	ResourceManager::Handler<ShaderSource> ResourceManager::load(ShaderSource&& source)
	{
		auto handler = load<ShaderSource>(m_sources, std::move(source));
		core::log::trace("ShaderSource {} generation {} LOADED", handler.index, handler.generation);
		return handler;
	}

	ResourceManager::Handler<Mesh> ResourceManager::load(Mesh&& mesh)
	{
		auto handler = load<Mesh>(m_meshes, std::move(mesh));
		core::log::trace("Mesh {} generation {} LOADED", handler.index, handler.generation);
		return handler;
	}

	ResourceManager::Handler<Material> ResourceManager::load(Material&& material)
	{
		auto handler = load<Material>(m_materials, std::move(material));
		core::log::trace("Material {} generation {} LOADED", handler.index, handler.generation);
		return handler;
	}

	ResourceManager::Handler<VertexLayout> ResourceManager::load(VertexLayout&& layout)
	{
		auto handler = load<VertexLayout>(m_layouts, std::move(layout));
		core::log::trace("VertexLayout {} generation {} LOADED", handler.index, handler.generation);
		return handler;
	}

	ResourceManager::Handler<Object> ResourceManager::load(Object&& object)
	{
		auto handler = load<Object>(m_objects, std::move(object));
		core::log::trace("Object {} generation {} LOADED", handler.index, handler.generation);
		return handler;
	}

	ResourceManager::Handler<Image> ResourceManager::load(Image&& image)
	{
		auto handler = load<Image>(m_images, std::move(image));
		core::log::trace("Image {} generation {} LOADED", handler.index, handler.generation);
		return handler;
	}


	void ResourceManager::unload(ResourceManager::Handler<ShaderSource> handler)
	{
		core::log::trace("ShaderSource {} generation {} UNLOADED", handler.index, handler.generation);
		unload<ShaderSource>(m_sources, handler);
	}

	void ResourceManager::unload(ResourceManager::Handler<Mesh> handler)
	{
		core::log::trace("Mesh {} generation {} UNLOADED", handler.index, handler.generation);
		unload<Mesh>(m_meshes, handler);
	}

	void ResourceManager::unload(ResourceManager::Handler<Material> handler)
	{
		core::log::trace("Material {} generation {} UNLOADED", handler.index, handler.generation);
		unload<Material>(m_materials, handler);
	}

	void ResourceManager::unload(ResourceManager::Handler<VertexLayout> handler)
	{
		core::log::trace("VertexLayout {} generation {} UNLOADED", handler.index, handler.generation);
		unload<VertexLayout>(m_layouts, handler);
	}

	void ResourceManager::unload(ResourceManager::Handler<Object> handler)
	{
		core::log::trace("Object {} generation {} UNLOADED", handler.index, handler.generation);
		unload<Object>(m_objects, handler);
	}

	void ResourceManager::unload(ResourceManager::Handler<Image> handler)
	{
		core::log::trace("Image {} generation {} UNLOADED", handler.index, handler.generation);
		unload<Image>(m_images, handler);
	}


	ResourceManager::Optional<ShaderSource> ResourceManager::get(ResourceManager::Handler<ShaderSource> handler)
	{
		return get<ShaderSource>(m_sources, handler);
	}

	ResourceManager::Optional<Mesh> ResourceManager::get(ResourceManager::Handler<Mesh> handler)
	{
		return get<Mesh>(m_meshes, handler);
	}

	ResourceManager::Optional<Material> ResourceManager::get(ResourceManager::Handler<Material> handler)
	{
		return get<Material>(m_materials, handler);
	}

	ResourceManager::Optional<VertexLayout> ResourceManager::get(ResourceManager::Handler<VertexLayout> handler)
	{
		return get<VertexLayout>(m_layouts, handler);
	}

	ResourceManager::Optional<Object> ResourceManager::get(ResourceManager::Handler<Object> handler)
	{
		return get<Object>(m_objects, handler);
	}

	ResourceManager::Optional<Image> ResourceManager::get(ResourceManager::Handler<Image> handler)
	{
		return get<Image>(m_images, handler);
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
		const auto slot = next<Asset>(container);

		if (slot == container.container.size())
			container.container.emplace_back(Slot<Asset>{.asset = Asset(std::forward<Args>(args)...)});
		else
		{
			container.container[slot].asset.emplace(std::forward<Args>(args)...);
			++container.container[slot].generation;
		}
		Handler<Asset> handler(slot, container.container[slot].generation);

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
		if (handler.index >= container.container.size() ||
				container.container[handler.index].generation != handler.generation ||
				!container.container[handler.index].asset)
			return std::nullopt;
		return container.container[handler.index].asset.value();
	}
}