export module bm.gfx:texture;

import std;

import bm.core;

namespace bm::gfx
{

	export class Texture
	{
	public:
	
		enum class Wrappering
		{
			Repeat,
			Mirrored_Repeat
		};
	
		enum class Filtering
		{
			Linear,
			Nearest
		};

		struct MetaData
		{
			int width = 0, height = 0, depth = 0, bytes_pp = 0;
			//Wrappering wrappering = Wrappering::Repeat;
			//Filtering filtering = Filtering::Nearest;
		};
	
	public:
	
		Texture(const bm::Image& image);
		
		~Texture();

		Texture() = delete;
		Texture(const Texture&) = delete;
		Texture& operator=(const Texture& oth) = delete;

		Texture(Texture&& oth) noexcept;
		Texture& operator=(Texture&& oth) noexcept;
	
		void bind(unsigned int tex_unit = 0) const noexcept;
		static void unbind();
	
		unsigned int getID() const noexcept { return m_id; }
	
		int getWidth() const noexcept { return m_meta.width; }
		int getHeight() const noexcept { return m_meta.height; }
		int getChannelsCount() const noexcept { return m_meta.bytes_pp; }

		template<typename... Args>
		static bm::Traits<Texture>::SPtr make(Args&&... args) { return std::make_shared<Texture>(std::forward<Args>(args)...); }
	
	private:
	
		void init() noexcept;
		void destroy() noexcept;
	
	private:
	
		unsigned int m_id;
		MetaData m_meta;
	};

	export struct TextureException : public Exception
	{
		TextureException(std::string_view what, Texture::MetaData data = {}) : Exception(what), data(data) {}

		Texture::MetaData data;
	};

}