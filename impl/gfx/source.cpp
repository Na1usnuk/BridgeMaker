module bm.gfx:source;
import :source;
import :shader;

import std;

namespace bm::gfx
{

	ShaderSource ShaderSource::fromFile(const std::filesystem::path& filepath)
	{
		std::fstream file(filepath, std::ios::in);
		if (not file.is_open())
			throw ShaderException(std::format("Failed to open file {}", filepath.string()));
		auto file_view = std::views::istream<char>(file);
		return ShaderSource(std::string(std::from_range, file_view));
	}
}