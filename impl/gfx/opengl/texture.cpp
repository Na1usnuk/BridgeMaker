module;

#include "glad/glad.h"

#define GL_VERIFY(func, ...) glVerify( std::source_location::current(), func, __VA_ARGS__)

module bm.gfx:texture;

import :texture;
import :verify;
import :context;

import std;

import bm.core;

namespace bm::gfx
{

	static GLenum getInternalGLFormat(int bpp)
	{
		switch (bpp)
		{
		case 1:  return GL_R8;
		case 3: return GL_SRGB8;
		case 4:  return  GL_SRGB8_ALPHA8;
		}
		throw TextureException("Unsupported texture format");
	}

	static GLenum getGLFormat(int bpp)
	{
		switch (bpp)
		{
		case 1:  return GL_RED;
		case 3: return GL_RGB;
		case 4: return GL_RGBA;
		}
		throw TextureException("Unsupported texture format");
	}

	void Texture::init() noexcept
	{
		const int gl_ver = Context::getCurrent().getVersion();

		if (gl_ver >= 45)
		{
			glCreateTextures(GL_TEXTURE_2D, 1, &m_id);
		}
		else
		{
			GL_VERIFY(glGenTextures, 1, &m_id);
			GL_VERIFY(glBindTexture, GL_TEXTURE_2D, m_id);
		}
	}

	Texture::Texture(const Image& image) :
		m_meta(image.getWidth(), image.getHeight(), 0, image.getChannelsCount())
	{
		init();

		const int gl_ver = Context::getCurrent().getVersion();

		if (gl_ver >= 45)
		{
			glTextureStorage2D(m_id, 1, getInternalGLFormat(m_meta.bytes_pp), m_meta.width, m_meta.height);
			glTextureSubImage2D(m_id, 0, 0, 0, m_meta.width, m_meta.height, getGLFormat(m_meta.bytes_pp), GL_UNSIGNED_BYTE, image.getData().data());
		}
		else
			GL_VERIFY(glTexImage2D, GL_TEXTURE_2D, 0, getInternalGLFormat(m_meta.bytes_pp), m_meta.width, m_meta.height, 0, getGLFormat(m_meta.bytes_pp), GL_UNSIGNED_BYTE, image.getData().data());
	}


	Texture::Texture(Texture&& oth) noexcept :
		m_id(std::exchange(oth.m_id, 0)),
		m_meta(std::exchange(oth.m_meta, MetaData{})) // Set oth data to defaults
	{
	}

	Texture& Texture::operator=(Texture&& oth) noexcept
	{
		if (this != &oth)
		{
			destroy();
			m_id = std::exchange(oth.m_id, 0);
			m_meta = std::exchange(oth.m_meta, MetaData{});
		}
		return *this;
	}

	Texture::~Texture()
	{
		if(m_id != 0)
			destroy();
	}

	void Texture::destroy() noexcept
	{
		core::log::trace("Texture {} destroyed", m_id);
		GL_VERIFY(glDeleteTextures, 1, &m_id);
	}

	void Texture::bind(unsigned int tex_unit) const noexcept
	{
		const int gl_ver = Context::getCurrent().getVersion();

		if (gl_ver >= 45)
			glBindTextureUnit(tex_unit, m_id);
		else
		{
			GL_VERIFY(glActiveTexture, GL_TEXTURE0 + tex_unit);
			GL_VERIFY(glBindTexture, GL_TEXTURE_2D, m_id);
		}
	}

	void Texture::unbind()
	{
		GL_VERIFY(glBindTexture, GL_TEXTURE_2D, 0);
	}

}