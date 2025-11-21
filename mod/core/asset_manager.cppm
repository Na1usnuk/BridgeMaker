export module bm.assetmanager;

import std;

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

		template<typename... Args>
		Traits<gfx::Shader>::SPtr loadShader(std::string name, Args&&... args)
		{
			if (m_shaders.contains(name) and not m_shaders[name].expired())
				return m_shaders[name].lock();

			log::core::trace("Shader '{}' created", name);

			auto shader = gfx::Shader::make(std::forward<Args>(args)...);
			m_shaders[name] = shader;
			return shader;
		}

		template<typename... Args>
		Traits<gfx::Texture>::SPtr loadTexture(std::string name, Args&&... args)
		{
			if (m_textures.contains(name) and not m_textures[name].expired())
				return m_textures[name].lock();
			else
				unloadTexture(name);

			log::core::trace("Texture '{}' created", name);

			auto texture = gfx::Texture::make(std::forward<Args>(args)...);
			m_textures[name] = texture;
			return texture;
		}

		void unloadTexture(const std::string& name)
		{
			m_textures.erase(name);
		}

		void unloadShader(const std::string& name)
		{
			m_shaders.erase(name);
		}


	private:

		AssetManager() = default;

	private:

		std::unordered_map<std::string, Traits<gfx::Shader>::WPtr> m_shaders;
		std::unordered_map<std::string, Traits<gfx::Texture>::WPtr> m_textures;

	};

}