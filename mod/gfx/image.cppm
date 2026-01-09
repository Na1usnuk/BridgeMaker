export module bm.gfx:image;

import bm.core;

import std;

namespace bm::gfx
{
	export class Image
	{
	public:

		// Can be converted to number of channels
		enum class Format
		{
			None = 0,
			Mono = 1,
			RGB  = 3,
			RGBA = 4
		};

		struct MetaData
		{
			int width = 0, height = 0;
			Format format = Format::None;
			std::filesystem::path filepath{};
		};

	public:

		Image(const std::filesystem::path& filepath);
		Image(const std::span<const std::byte> data, int width, int height, Format format);

		Image() = delete;
		Image(const Image&) = delete;
		Image& operator=(const Image&) = delete;

		Image& operator=(Image&&) noexcept = default;
		Image(Image&&) noexcept = default;
		~Image() = default;

		const std::span<const std::byte> getData() const noexcept { return { m_data.get(), m_size}; }
		std::span<std::byte> getData() noexcept { return { m_data.get(), m_size }; }
		const std::filesystem::path& getFilepath() const noexcept { return m_meta.filepath; }
		int getWidth() const noexcept { return m_meta.width; }
		int getHeight() const noexcept { return m_meta.height; }
		std::size_t getSize() const noexcept { return m_size; }
		Format getFormat() const noexcept { return m_meta.format; }
		int getChannelsCount() const noexcept { return static_cast<int>(m_meta.format); }

		void flipVertically() noexcept;
		void flipHorizontally() noexcept;

	private:

		MetaData m_meta;
		std::unique_ptr<std::byte, void(*)(void*)> m_data; // stbi_image_free or delete[]
		std::size_t m_size;
	};


	// Exception thrown on image loading errors
	export struct ImageException : public Exception
	{
		ImageException(std::string_view what, Image::MetaData meta) : Exception(what), metadata(meta) {}
		Image::MetaData metadata;
	};
}