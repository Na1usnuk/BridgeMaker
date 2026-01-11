export module bm.core:handle;

import std;

import :verify;

namespace bm
{

	// Handle to resource. Generation 0 mean invalid
	export template<class T>
		struct [[nodiscard]] Handle
	{
		std::uint32_t index = 0, generation = 0;

		bool operator==(const Handle<T>& oth) const noexcept { return index == oth.index and generation == oth.generation; }
		bool operator!=(const Handle<T>& oth) const noexcept { return not(*this == oth); }

		void invalidate() noexcept { generation = index = 0; }
		bool isValid() const noexcept { return generation not_eq 0; }
		operator bool() const noexcept { return isValid(); }
	};

	export struct HandleHash
	{
		template<class T>
		std::size_t operator()(const Handle<T>& h) const noexcept
		{
			return std::hash<std::uint64_t>{}((std::uint64_t(h.index) << 32) | h.generation);
		}
	};

	export template<class T>
		requires std::is_object_v<T> and std::is_move_constructible_v<T>
		class HandleStorage
	{
	public:

		using Handle = Handle<T>;

		using OptionalRef = std::optional<std::reference_wrapper<T>>;
		using ConstOptionalRef = std::optional<std::reference_wrapper<const T>>;

	private:

		struct Slot
		{
			std::optional<T> asset;
			std::uint32_t generation = 1;
		};

		struct Storage
		{
			std::vector<Slot> slots;
			std::vector<std::uint32_t> free_slots;
		};

	public:

		template<typename... Args>
		Handle load(Args&&... args)
		{
			const auto slot = nextSlot();

			if (slot == m_storage.slots.size())
				m_storage.slots.emplace_back(Slot{ .asset = T(std::forward<Args>(args)...) });
			else
				m_storage.slots[slot].asset.emplace(std::forward<Args>(args)...);

			Handle handle{ slot, m_storage.slots[slot].generation };

			return handle;
		}

		void unload(Handle handle) noexcept
		{
			core::verify(m_storage.slots.size() > handle.index, "HandleStorage::unload verify failed: out of range handle");

			if (m_storage.slots[handle.index].generation != handle.generation)
			{
				core::log::warning("HandleStorage::unload ignored: stale handle {}", handle.index);
				return;
			}

			m_storage.slots[handle.index].asset.reset();
			m_storage.slots[handle.index].generation++;
			m_storage.free_slots.push_back(handle.index);
		}

		const T& get(Handle handle) const noexcept
		{
			core::verify(handle.index < m_storage.slots.size() and
				m_storage.slots[handle.index].generation == handle.generation and
				m_storage.slots[handle.index].asset.has_value(),
				"HandleStorage::get verify failed: cannot get resource by handle {}", handle.index);

			return m_storage.slots[handle.index].asset.value();
		}

		T& get(Handle handle) noexcept
		{
			return const_cast<T&>(static_cast<const HandleStorage*>(this)->get(handle));
		}

		ConstOptionalRef tryGet(Handle handle) const noexcept
		{
			if (handle.index >= m_storage.slots.size() or
				m_storage.slots[handle.index].generation != handle.generation or
				!m_storage.slots[handle.index].asset)
				return std::nullopt;
			return std::cref(m_storage.slots[handle.index].asset.value());
		}

		OptionalRef tryGet(Handle handle) noexcept
		{
			auto result = static_cast<const HandleStorage*>(this)->tryGet(handle);
			if (!result)
				return std::nullopt;
			return std::ref(const_cast<T&>(result->get()));
		}

	private:

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