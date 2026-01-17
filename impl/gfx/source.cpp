module bm.gfx:source;
import :source;
import :shader;

import bm.core;

import std;

namespace bm::gfx
{

	ShaderSource ShaderSource::fromFile(const std::filesystem::path& filepath, Stage stage)
	{
		std::ifstream file(filepath, std::ios::binary);
		if (not file.is_open())
			throw ShaderException(std::format("Failed to open file {}", filepath.string()));
		file >> std::noskipws;
		auto file_view = std::views::istream<char>(file);
		std::string src(std::from_range, file_view);
		core::log::trace("{}",src);
		return ShaderSource(std::move(src), stage);
	}
}