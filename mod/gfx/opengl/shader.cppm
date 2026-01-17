export module bm.gfx:shader;

import :source;

import bm.core;

import std;
import glm;


namespace bm::gfx
{

	export class Shader
	{
	public:

		Shader(const ShaderSource& source);
		~Shader();

		Shader(Shader&& oth) noexcept;
		Shader& operator=(Shader&& oth) noexcept;

		Shader(const Shader&) = delete;
		Shader& operator=(const Shader&) = delete;

		unsigned int getId() const noexcept { return m_id; }
		ShaderSource::Stage getStage() const noexcept { return m_stage; }

	private:

		void destroy() noexcept;

	private:

		unsigned int m_id;
		ShaderSource::Stage m_stage;

	};

	export class ShaderProgram
	{
	public:

		ShaderProgram(const Shader& vertex_shader, const Shader& fragment_shader);
	
		~ShaderProgram();
	
		ShaderProgram(const ShaderProgram&) = delete;
		ShaderProgram& operator=(const ShaderProgram&) = delete;
		ShaderProgram(ShaderProgram&& oth) noexcept;
		ShaderProgram& operator=(ShaderProgram&& oth) noexcept;
	
		void bind() const;
		static void unbind();
	
		unsigned int getId() const { return m_id; }
	
		void setUniform(std::string_view name, float f0, float f1, float f2, float f3);
		void setUniform(std::string_view name, float f0, float f1, float f2);
		void setUniform(std::string_view name, float f);
		void setUniform(std::string_view name, int i);
		void setUniform(std::string_view name, bool b);
		void setUniform(std::string_view name, const glm::vec3& vec);
		void setUniform(std::string_view name, const glm::vec4& vec);
		void setUniform(std::string_view name, const glm::mat4& mat);
		void setUniform(std::string_view name, const int* values, std::size_t count);
	
	private:
	
		int getUniformLocation(std::string_view);
		void destroy() noexcept;
	
	private:
	
		unsigned int m_id;
		std::unordered_map<std::string, int> m_uniform_cache;
	};

	export struct ShaderException : public core::Exception
	{
		ShaderException(std::string_view what) : core::Exception(what) {}
	};

}

