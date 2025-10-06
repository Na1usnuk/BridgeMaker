export module bm.gfx.texture;

import bm.gfx.asset;

import std;

namespace bm::gfx
{

export class Texture : public Asset
{
public:

	using Ptr = std::shared_ptr<Texture>;
	using KPtrRef = const Ptr&;

	enum class Wrappering
	{
		REPEAT = 0x2901,
		MiRRORED_REPEAT = 0x8370
	};

	enum class Filtering
	{
		LINEAR = 0x2601,
		NEAREST = 0x2600
	};

	struct Data
	{
		int width, height, depth, bpp;
		std::filesystem::path file_path;
		unsigned char* buffer;
	};

public:

	Texture(const std::filesystem::path& filepath);
	Texture();
	~Texture();

	void bind(unsigned int tex_unit = 0) const;
	void unbind() const;

	static void setDefaultWrappering(Wrappering wrap = Wrappering::REPEAT);
	static void setDefaultFiltering(Filtering wrap = Filtering::NEAREST);

	void setWrappering(bool wrap_s, bool wrap_t, Wrappering wrappering) const;
	void setFiltering(bool wrap_s, bool wrap_t, Filtering filtering) const;

	template<typename... Args>
	static Ptr make(Args&&... args) { return std::make_shared<Texture>(std::forward<Args>(args)...); }

private:

	unsigned int m_id;
	Data m_data;

	static Texture::Wrappering s_default_wrappering;
	static Texture::Filtering s_default_filtering;
};

export using TexturePtr = Texture::Ptr;

}