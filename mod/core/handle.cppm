export module bm.core:handle;

import std;

import :verify;
import :log;

namespace bm
{

	// Handle to resource. Generation 0 mean invalid
	export template<class T>
		struct [[nodiscard]] Handle
	{
		constexpr Handle() noexcept = default;
		constexpr Handle(std::uint32_t index, std::uint32_t generation) noexcept
			: index(index), generation(generation)
		{}

		std::uint32_t index = 0, generation = 0;

		constexpr bool operator==(const Handle<T>& oth) const noexcept { return index == oth.index and generation == oth.generation; }
		constexpr bool operator!=(const Handle<T>& oth) const noexcept { return not(*this == oth); }

		constexpr void invalidate() noexcept { generation = index = 0; }
		constexpr bool isValid() const noexcept { return generation not_eq 0; }
		constexpr operator bool() const noexcept { return isValid(); }
	};

	// Hash for Handle to use in unordered containers
	export struct HandleHash
	{
		template<class T>
		std::size_t operator()(const Handle<T>& h) const noexcept
		{
			return std::hash<std::uint64_t>{}((std::uint64_t(h.index) << 32) | h.generation);
		}
	};

	// Storage for resources managed by Handle
	export template<class K, class V = K>
		// V must be movable because storage may relocate elements
		requires std::is_object_v<V> and std::is_move_constructible_v<V>
	class HandleStorage
	{
	public:

		using Handle = Handle<K>;

		using OptionalRef = std::optional<std::reference_wrapper<V>>;
		using ConstOptionalRef = std::optional<std::reference_wrapper<const V>>;

	private:

		struct Slot
		{
			std::optional<V> asset;
			std::uint32_t generation = 1;
		};

		struct Storage
		{
			std::vector<Slot> slots;
			std::vector<std::uint32_t> free_slots;
		};

	public:

		HandleStorage() = default;
		~HandleStorage() = default;

		HandleStorage(const HandleStorage&) = delete;
		HandleStorage& operator=(const HandleStorage&) = delete;

		HandleStorage(HandleStorage&&) noexcept = default;
		HandleStorage& operator=(HandleStorage&&) noexcept = default;

		// Load asset and return handle
		template<typename... Args>
			requires std::constructible_from<V, Args...>
		Handle load(Args&&... args)
		{
			const auto slot = nextSlot();

			if (slot == m_storage.slots.size())
				m_storage.slots.emplace_back(Slot{ .asset = V(std::forward<Args>(args)...) });
			else
				m_storage.slots[slot].asset.emplace(std::forward<Args>(args)...);

			core::log::trace("HandleStorage::load: loaded resource {}", slot);

			Handle handle{ slot, m_storage.slots[slot].generation };

			return handle;
		}

		// Unload asset by handle
		void unload(Handle handle) noexcept
		{
			core::verify(handle.isValid(), "HandleStorage::unload verify failed: invalid handle");
			core::verify(m_storage.slots.size() > handle.index, "HandleStorage::unload verify failed: out of range handle");
			core::verify(m_storage.slots[handle.index].generation == handle.generation, 
				"HandleStorage::unload verify failed: stale handle {}", handle.index);

			m_storage.slots[handle.index].asset.reset();
			m_storage.slots[handle.index].generation++;
			m_storage.free_slots.push_back(handle.index);

			core::log::trace("HandleStorage::unload: unloaded handle {}", handle.index);
		}

		// Unload all assets
		// Note: invalidates all existing handles
		void unload() noexcept
		{
			m_storage.slots.clear();
			m_storage.free_slots.clear();

			//m_storage.slots.shrink_to_fit();
			//m_storage.free_slots.shrink_to_fit();

			core::log::trace("HandleStorage::unload: unloaded all resources");
		}

		// Get asset by handle. Unsafe if handle is invalid, but faster than tryGet
		const V& get(Handle handle) const noexcept
		{
			core::verify(handle.isValid(), "HandleStorage::get verify failed: invalid handle");
			core::verify(m_storage.slots.size() > handle.index, "HandleStorage::get verify failed: out of range handle");
			core::verify(m_storage.slots[handle.index].generation == handle.generation,
				"HandleStorage::get verify failed: stale handle {}", handle.index);
			core::verify(m_storage.slots[handle.index].asset.has_value(),
				"HandleStorage::get verify failed: empty slot {}", handle.index);

			return m_storage.slots[handle.index].asset.value();
		}

		V& get(Handle handle) noexcept
		{
			return const_cast<V&>(static_cast<const HandleStorage*>(this)->get(handle));
		}

		// Try get asset by handle. Return nullopt if handle is invalid
		ConstOptionalRef tryGet(Handle handle) const noexcept
		{
			if (handle.index >= m_storage.slots.size() or
				m_storage.slots[handle.index].generation != handle.generation or
				not m_storage.slots[handle.index].asset.has_value())
				return std::nullopt;
			return std::cref(m_storage.slots[handle.index].asset.value());
		}

		OptionalRef tryGet(Handle handle) noexcept
		{
			auto result = static_cast<const HandleStorage*>(this)->tryGet(handle);
			if (!result)
				return std::nullopt;
			return std::ref(const_cast<V&>(result->get()));
		}

		bool contains(Handle handle) const noexcept
		{
			return tryGet(handle).has_value();
		}

	private:

		// Helper. Get next available slot index
		std::uint32_t nextSlot() noexcept
		{
			if (not m_storage.free_slots.empty())
			{
				auto slot = m_storage.free_slots.back();
				m_storage.free_slots.pop_back();
				return slot;
			}
			return m_storage.slots.size();
		}

	private:

		Storage m_storage;
	};


	// Handle RAII wrapper to automatically unload asset
	// To raw to use now. TODO: make smart haldle system usable and safe
	export template<class T>
		class [[nodiscard]] SmartHandle
	{
	public:

		SmartHandle(Handle<T> handle, HandleStorage<T>& handle_storage) noexcept : m_handle(handle), m_handle_storage(&handle_storage) {}
		SmartHandle(HandleStorage<T>& handle_storage, Handle<T> handle) noexcept : m_handle(handle), m_handle_storage(&handle_storage) {}
		~SmartHandle() { m_handle_storage->unload(m_handle); }

		SmartHandle(const SmartHandle&) = delete;
		void operator=(const SmartHandle&) = delete;

		SmartHandle(SmartHandle&& other) noexcept
			: m_handle(std::exchange(other.m_handle, Handle<T>{}))
			, m_handle_storage(other.m_handle_storage)
		{}

		SmartHandle& operator=(SmartHandle&& other) noexcept
		{
			if (this != &other)
			{
				m_handle_storage.unload(m_handle);
				m_handle = std::exchange(other.m_handle, Handle<T>{});
				m_handle_storage = other.m_handle_storage;
			}
			return *this;
		}

		Handle<T> getHandle() const noexcept { return m_handle; }
		HandleStorage<T>& getHandleStorage() const noexcept { return *m_handle_storage; }

	private:

		Handle<T> m_handle;
		HandleStorage<T>* m_handle_storage;
	};
}