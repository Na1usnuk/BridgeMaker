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

	int Texture::s_num_of_slots = 0;

	int Texture::getMaxSlotCount()
	{
		if (s_num_of_slots == 0)
			glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &s_num_of_slots);

		return s_num_of_slots;
	}

	Texture::Texture(const std::filesystem::path& filepath)
		: m_id(0), m_data(0, 0, 0, 0, filepath, nullptr)
	{

		stbi_set_flip_vertically_on_load(true);
		m_data.buffer = stbi_load(filepath.string().c_str(), &m_data.width, &m_data.height, &m_data.bpp, 4);
		//core::verify(m_data.buffer, "Failed to load \"" + filepath.string() + "\" while creating texture! Reason: " + stbi_failure_reason());

		glCall(glGenTextures, 1, &m_id);
		glCall(glBindTexture, GL_TEXTURE_2D, m_id);

		const int levels = 1 + std::floor(std::log2(std::max(m_data.width, m_data.height)));

		glCall(glTexStorage2D, GL_TEXTURE_2D, levels, GL_RGBA8, m_data.width, m_data.height);
		glCall(glTexSubImage2D, GL_TEXTURE_2D, 0, 0, 0, m_data.width, m_data.height, GL_RGBA, GL_UNSIGNED_BYTE, m_data.buffer);

		glCall(glGenerateMipmap, GL_TEXTURE_2D);

		//setWrappering(true, true, s_default_wrappering);
		//setFiltering(true, true, s_default_filtering);

		stbi_image_free(m_data.buffer);
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