export module bm.gfx:source;

import std;


namespace bm::gfx
{
	// Represent CPU side shader source. 
	// Simple string wrapper for now. Additional functionality will be added in future
	export class ShaderSource
	{
	public:

		static ShaderSource fromFile(const std::filesystem::path& filepath);
		static ShaderSource fromString(std::string_view source) { return { source }; }

		ShaderSource(std::string_view source) : m_source(source) {}
		ShaderSource(std::string&& source) : m_source(std::move(source)) {}

		ShaderSource(ShaderSource&&) = default;
		ShaderSource& operator=(ShaderSource&&) = default;
		ShaderSource(const ShaderSource&) = delete;
		ShaderSource& operator=(const ShaderSource&) = delete;
		~ShaderSource() = default;

		const std::string& getSource() const { return m_source; }

	private:

		std::string m_source;

	};
}