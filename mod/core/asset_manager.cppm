module;
#include "bmpch.hpp"
export module bm.assetmanager;

import bm.gfx.asset;
import bm.gfx.shader;
import bm.gfx.texture;

import bm.traits;

import bm.log;

namespace bm
{

	export class AssetManager
	{
	public:

		static AssetManager& get() 
		{ 
			static AssetManager asset_manager; 
			return asset_manager; 
		}

		//template<typename AssetType, typename... Args>
		//requires std::is_base_of_v<gfx::Asset, AssetType>
		//std::future<gfx::Traits<AssetType>::Ptr> loadAsync(const std::string& name, Args&&... args)
		//{
		//	if (m_assets.contains(name) and not m_assets[name].expired())
		//		return std::make_ready_future(
		//			std::static_pointer_cast<AssetType>(m_assets[name].lock()));
		//	else
		//		unload<AssetType>(name);



		//}

		template<typename AssetType, typename... Args>
		requires std::is_base_of_v<gfx::Asset, AssetType>
		Traits<AssetType>::Ptr load(const std::string& name, Args&&... args)
		{
			if (m_assets.contains(name) and not m_assets[name].expired())
				return std::static_pointer_cast<AssetType>(m_assets[name].lock());
			else
				unload<AssetType>(name);

			log::core::trace("Asset '{}' created", name);

			auto asset = AssetType::make(std::forward<Args>(args)...);
			m_assets[name] = asset;
			return asset;
		}

		template<typename AssetType>
		requires std::is_base_of_v<gfx::Asset, AssetType>
		void unload(const std::string& name)
		{
			m_assets.erase(name);
		}

	private:

		AssetManager() = default;

	private:

		std::unordered_map<std::string, std::weak_ptr<gfx::Asset>> m_assets;

	};

}