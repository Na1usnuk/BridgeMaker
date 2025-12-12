export module bm.gfx:material;

import :material;
import :asset_manager;

namespace bm::gfx
{
	Material::Material() : Material(::bm::AssetManager::get().loadShader("basic")) {}
	Material::Material(Traits<Shader>::KSPtrRef shader) : m_shader(shader) { m_texture = ::bm::AssetManager::get().loadTexture("nothing"); }
	Material::Material(Traits<Shader>::KSPtrRef shader, Traits<Texture>::KSPtrRef texture) : m_shader(shader), m_texture(texture) {}
	Material::Material(Traits<Texture>::KSPtrRef texture) : Material(::bm::AssetManager::get().loadShader("basic"), texture) {}
}