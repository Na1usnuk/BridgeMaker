module;

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

module bm.gfx:image;

import :image;

import std;

namespace bm::gfx
{
	Image::Image(const std::filesystem::path& filepath) :
		m_meta(0, 0, Format::None, filepath),
		m_data( nullptr, stbi_image_free)
	{
		int bpp;

		auto buff = stbi_load(filepath.string().c_str(), &m_meta.width, &m_meta.height, &bpp, 0);

		if (not buff)
		{
			std::string_view reason = stbi_failure_reason();
			throw ImageException(std::format("Failed to load image: {}", reason), m_meta);
		}
		
		switch (bpp)
		{
		case 1: m_meta.format = Format::Mono; break;
		case 3: m_meta.format = Format::RGB; break;
		case 4: m_meta.format = Format::RGBA; break;
		default: stbi_image_free(buff); throw ImageException("Unknown image pixel format", m_meta);
		}

		m_data.reset(reinterpret_cast<std::byte*>(buff));
		m_size = static_cast<std::size_t>(m_meta.width) * m_meta.height * bpp;
	}

	Image::Image(const std::span<const std::byte> data, int width, int height, Format format) :
		m_meta{ width, height, format, "" },
		m_data{ new std::byte[data.size()], [](void* p) noexcept { delete[](p); } },
		m_size(data.size())
	{
		if (format == Format::None) 
			throw ImageException("Unknown image pixel format", m_meta);

		if (data.size() != static_cast<std::size_t>(width) * height * getChannelsCount())
			throw ImageException("Incorect image size", m_meta);

		std::memcpy(m_data.get(), data.data(), data.size());
	}

	void Image::flipVertically() noexcept
	{
		auto data = getData();
		const std::size_t row_size = static_cast<std::size_t>(m_meta.width) * getChannelsCount();

		for (int y = 0; y < m_meta.height / 2; ++y)
		{
			auto row_top = data.subspan(static_cast<std::size_t>(y) * row_size, row_size);
			auto row_bottom = data.subspan(static_cast<std::size_t>(m_meta.height - 1 - y) * row_size, row_size);

			std::ranges::swap_ranges(row_top, row_bottom);
		}
	}

	void Image::flipHorizontally() noexcept
	{
		auto data = getData();
		const int channels = getChannelsCount();
		const std::size_t row_size = static_cast<std::size_t>(m_meta.width) * channels;

		for (int y = 0; y < m_meta.height; ++y)
		{
			auto row = data.subspan(static_cast<std::size_t>(y) * row_size, row_size);

			for (int x = 0; x < m_meta.width / 2; ++x)
			{
				auto lhs = row.subspan(static_cast<std::size_t>(x) * channels, channels);
				auto rhs = row.subspan(static_cast<std::size_t>(m_meta.width - 1 - x) * channels,channels);

				std::ranges::swap_ranges(lhs, rhs);
			}
		}
	}


}