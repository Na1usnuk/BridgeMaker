export module bm.gfx.texture;

import std;

namespace bm::gfx
{

export class Texture
{
public:

	enum class Type : unsigned int
	{
		_1D = 0x0DE0,
		_2D = 0x0DE1,
		_3D = 0x806F
	};

	enum class FileType
	{
		JPEG = 3, 
		JPG  = 3, 
		PNG	 = 4,
		WEBP = 4
	};

	enum class Wrappering : unsigned int
	{
		REPEAT = 0x2901,
		MiRRORED_REPEAT = 0x8370
	};

	enum class Filtering : unsigned int
	{
		LINEAR = 0x2601,
		NEAREST = 0x2600
	};

	struct Data
	{
		int width, height, depth, bpp;
		std::string file_path;
		Type texture_type;
		unsigned char* buffer;
	};

public:

	Texture(const std::string& file_path, Type texture_type = Type::_2D);
	~Texture();

	void bind(unsigned int tex_unit = 0) const;
	void unbind() const;

	static void setDefaultWrappering(Wrappering wrap = Wrappering::REPEAT);
	static void setDefaultFiltering(Filtering wrap = Filtering::NEAREST);

	void setBorder(bool wrap_s, bool wrap_t, std::array<float, 4> color = {.0f, .0f, .0f, 1.f}) const;
	void setWrappering(bool wrap_s, bool wrap_t, Wrappering wrappering) const;
	void setFiltering(bool wrap_s, bool wrap_t, Filtering filtering) const;

private:

	unsigned int m_id;
	Data m_data;

	static Texture::Wrappering s_default_wrappering;
	static Texture::Filtering s_default_filtering;
};

}