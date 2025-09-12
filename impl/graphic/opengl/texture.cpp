module;

#include "stb_image.h"
#include "glad/glad.h"

module bm.gfx.texture;

import std;

import bm.assert;
import bm.log;
import bm.gfx.utility;

namespace bm::gfx
{

	Texture::Wrappering Texture::s_default_wrappering = Texture::Wrappering::REPEAT;
	Texture::Filtering Texture::s_default_filtering = Texture::Filtering::NEAREST;

	Texture::Texture(const std::string& file_path, Type texture_type)
		: m_id(0), m_data(0, 0, 0, 0, "", texture_type, nullptr)
	{
		FileType file_type;
		std::string type_str = file_path.substr(file_path.rfind(".") + 1, file_path.size());
		std::transform(type_str.begin(), type_str.end(), type_str.begin(), [](const unsigned char& c) { return std::tolower(c); });

		if ("png" == type_str)
			file_type = FileType::PNG;
		else if ("jpg" == type_str || "jpeg" == type_str)
			file_type = FileType::JPG;
		else if ("webp" == type_str)
			file_type = FileType::WEBP;
		else {
			core::verify(false, "Unsupported file type: " + type_str);
		}

		stbi_set_flip_vertically_on_load(true);
		m_data.buffer = stbi_load(file_path.c_str(), &m_data.width, &m_data.height, &m_data.bpp, (int)file_type);
		core::verify(m_data.buffer, "Failed to load \"" + file_path + "\" while creating texture! Reason: " + stbi_failure_reason());

		glCall(glGenTextures, 1, &m_id);
		glCall(glBindTexture, (GLenum)m_data.texture_type, m_id);

		const int levels = 1 + std::floor(std::log2(std::max(m_data.width, m_data.height)));

		int internal_format, format;
		switch (file_type)
		{
			//case FileType::JPEG:
		case FileType::JPG:
		{
			internal_format = GL_RGB8;
			format = GL_RGB;
			break;
		}
		//case FileType::WEBP:
		case FileType::PNG:
		{
			internal_format = GL_RGBA8;
			format = GL_RGBA;
			break;
		}
		}

		switch (texture_type)
		{
		case Type::_1D:
		{
			core::verify(false, "Not supported!");
			glCall(glTextureStorage1D, m_id, levels, internal_format, m_data.width);
			glCall(glTextureSubImage1D, m_id, 0, 0, m_data.width, format, GL_UNSIGNED_BYTE, m_data.buffer);
			break;
		}
		case Type::_2D:
		{
			glCall(glTextureStorage2D, m_id, levels, internal_format, m_data.width, m_data.height);
			glCall(glTextureSubImage2D, m_id, 0, 0, 0, m_data.width, m_data.height, format, GL_UNSIGNED_BYTE, m_data.buffer);
			break;
		}
		case Type::_3D:
		{
			core::verify(false, "Not supported!");
			glCall(glTextureStorage3D, m_id, levels, internal_format, m_data.width, m_data.height, m_data.depth);
			glCall(glTextureSubImage3D, m_id, 0, 0, 0, 0, m_data.width, m_data.height, m_data.depth, format, GL_UNSIGNED_BYTE, m_data.buffer);
			break;
		}

		}
		glCall(glGenerateTextureMipmap, m_id);

		setWrappering(true, true, s_default_wrappering);
		setFiltering(true, true, s_default_filtering);

		stbi_image_free(m_data.buffer);
	}

	void Texture::setBorder(const bool wrap_s, bool wrap_t, std::array<float, 4> color) const
	{
		if (wrap_s)
			glCall(glTextureParameteri, m_id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		if (wrap_t)
			glCall(glTextureParameteri, m_id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

		glCall(glTextureParameterfv, m_id, GL_TEXTURE_BORDER_COLOR, color.data());
	}

	void Texture::setWrappering(bool wrap_s, bool wrap_t, Wrappering wrappering) const
	{
		if (wrap_s)
			glCall(glTextureParameteri, m_id, GL_TEXTURE_WRAP_S, (int)wrappering);
		if (wrap_t)
			glCall(glTextureParameteri, m_id, GL_TEXTURE_WRAP_T, (int)wrappering);
	}

	void Texture::setFiltering(bool filter_mag, bool filter_min, Filtering filtering) const
	{
		if (filter_mag)
			glCall(glTextureParameteri, m_id, GL_TEXTURE_MAG_FILTER, (int)filtering);
		if (filter_min)
			glCall(glTextureParameteri, m_id, GL_TEXTURE_MIN_FILTER, (int)filtering);
	}

	Texture::~Texture()
	{
		glCall(glDeleteTextures, 1, &m_id);
	}

	void Texture::bind(unsigned int tex_unit) const
	{
		glCall(glActiveTexture, GL_TEXTURE0 + tex_unit);
		glCall(glBindTexture, GL_TEXTURE_2D, m_id);
	}

	void Texture::unbind() const
	{
		glCall(glBindTexture, GL_TEXTURE_2D, 0);
	}

	void Texture::setDefaultWrappering(Wrappering wrapping)
	{
		s_default_wrappering = wrapping;
	}

	void Texture::setDefaultFiltering(Filtering filtering)
	{
		s_default_filtering = filtering;
	}

}