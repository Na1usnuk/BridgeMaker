module;

#include "stb_image.h"
#include "glad/glad.h"

#define GL_CALL(func, ...) glCallImpl( std::source_location::current(), func, __VA_ARGS__)

module bm.gfx:texture;

import :texture;

import std;

import bm.core;
import :utility;

namespace bm::gfx
{

	Texture::Wrappering Texture::s_default_wrappering = Texture::Wrappering::REPEAT;
	Texture::Filtering Texture::s_default_filtering = Texture::Filtering::NEAREST;

	Texture::Texture(const std::filesystem::path& filepath)
		: m_id(0), m_data(0, 0, 0, 0, filepath, nullptr)
	{

		stbi_set_flip_vertically_on_load(true);
		m_data.buffer = stbi_load(filepath.string().c_str(), &m_data.width, &m_data.height, &m_data.bpp, 4);

		const char* reason = stbi_failure_reason();
		std::string reason_str = reason ? reason : "unknown reason";

		core::verify(
			m_data.buffer,
			std::format("Failed to load {} while creating texture! Reason: {}",
			filepath.string(),
			reason_str)
		);


		GL_CALL(glGenTextures, 1, &m_id);
		GL_CALL(glBindTexture, GL_TEXTURE_2D, m_id);

		const int levels = 1 + std::floor(std::log2(std::max(m_data.width, m_data.height)));

		GL_CALL(glTexStorage2D, GL_TEXTURE_2D, levels, GL_RGBA8, m_data.width, m_data.height);
		GL_CALL(glTexSubImage2D, GL_TEXTURE_2D, 0, 0, 0, m_data.width, m_data.height, GL_RGBA, GL_UNSIGNED_BYTE, m_data.buffer);

		GL_CALL(glGenerateMipmap, GL_TEXTURE_2D);

		//setWrappering(true, true, s_default_wrappering);
		//setFiltering(true, true, s_default_filtering);

		stbi_image_free(m_data.buffer);
	}

	Texture::Texture()
		: m_id(0), m_data(1, 1, 0, 4, "", nullptr)
	{
		unsigned char white_pixel[4] = { 255, 255, 255, 255 };

		GL_CALL(glGenTextures, 1, &m_id);
		GL_CALL(glBindTexture, GL_TEXTURE_2D, m_id);

		GL_CALL(glTexImage2D, GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, white_pixel);

		GL_CALL(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		GL_CALL(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		GL_CALL(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		GL_CALL(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}

	void Texture::setWrappering(bool wrap_s, bool wrap_t, Wrappering wrappering) const
	{
		if (wrap_s)
			GL_CALL(glTextureParameteri, m_id, GL_TEXTURE_WRAP_S, (int)wrappering);
		if (wrap_t)
			GL_CALL(glTextureParameteri, m_id, GL_TEXTURE_WRAP_T, (int)wrappering);
	}

	void Texture::setFiltering(bool filter_mag, bool filter_min, Filtering filtering) const
	{
		if (filter_mag)
			GL_CALL(glTextureParameteri, m_id, GL_TEXTURE_MAG_FILTER, (int)filtering);
		if (filter_min)
			GL_CALL(glTextureParameteri, m_id, GL_TEXTURE_MIN_FILTER, (int)filtering);
	}

	Texture::~Texture()
	{
		GL_CALL(glDeleteTextures, 1, &m_id);
	}

	void Texture::bind(unsigned int tex_unit) const
	{
		GL_CALL(glActiveTexture, GL_TEXTURE0 + tex_unit);
		GL_CALL(glBindTexture, GL_TEXTURE_2D, m_id);
	}

	void Texture::unbind() const
	{
		GL_CALL(glBindTexture, GL_TEXTURE_2D, 0);
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