export module bm.gfx:utility;

import bm.core;
import std;

namespace bm::gfx
{

	// A handle to an asset managed by the ResourceManager
	export template<class Asset>
		struct [[nodiscard]] Handler
	{
		std::uint32_t index = 0, generation = 0;
		bool operator==(const Handler<Asset>& oth) const { return index == oth.index and generation == oth.generation; }
		bool operator!=(const Handler<Asset>& oth) const { return not(*this == oth); }
	};

	// Handler RAII wrapper to automatically unload asset
	//export template<class Asset>
	//	class [[nodiscard]] SmartHandler
	//{
	//public:
	//	SmartHandler(Handler<Asset> handler) : m_handler(handler) {}
	//	~SmartHandler() { ResourceManager::unload(m_handler); }

	//	SmartHandler(SmartHandler&&) = default;
	//	SmartHandler(const SmartHandler&) = delete;
	//	SmartHandler& operator=(SmartHandler&&) = default;
	//	SmartHandler& operator=(const SmartHandler&) = delete;
	//private:
	//	Handler<Asset> m_handler;
	//};

	export struct HandlerHash
	{
		template<class Asset>
		std::size_t operator()(const Handler<Asset>& h) const noexcept
		{
			return (std::uint64_t(h.index) << 32) | std::uint64_t(h.generation);
		}
	};

	// Used to specify the expected usage pattern of the data store
	export enum class Usage
	{
		Const,	 // For immutable data
		Static,  // Data changes rarelly
		Dynamic, // Data can change sometimes
		Stream,  // Data changes frequently 
	};
	
}



