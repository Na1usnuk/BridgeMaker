export module bm.gfx:source;

import :utility;

import std;


namespace bm::gfx
{
	// Represent CPU side shader source. 
	// Simple string wrapper for now. Additional functionality will be added in future
	export class ShaderSource
	{
	public:

		enum class Stage
		{
			Vertex,
			Fragment
		};

	public:

		static ShaderSource fromFile(const std::filesystem::path& filepath, Stage stage);
		static ShaderSource fromString(std::string source, Stage stage) { return { std::move(source), stage }; } // Just for convenience

		ShaderSource(std::string source, Stage stage) : 
			m_source(std::move(source)),
			m_stage(stage)
		{}

		std::string_view getSource() const noexcept { return m_source; }
		const Stage getStage() const noexcept { return m_stage; }

	private:

		std::string m_source;
		Stage m_stage;

	};
}