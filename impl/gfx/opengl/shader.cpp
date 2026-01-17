module;

#include "glad/glad.h"

#define GL_VERIFY(func, ...) glVerify( std::source_location::current(), func, __VA_ARGS__)

module bm.gfx:shader;

import :shader;
import :verify;

import bm.config;
import bm.core;

import glm;
import std;


namespace bm::gfx
{
	// Returns error string on fail
	static std::optional<std::string> checkShaderCompilationStatus(unsigned int shader_id)
	{
		int result;
		GL_VERIFY(glGetShaderiv, shader_id, GL_COMPILE_STATUS, &result);
		if (result == GL_FALSE)
		{
			int size;
			GL_VERIFY(glGetShaderiv, shader_id, GL_INFO_LOG_LENGTH, &size);
			std::string msg( size, '\0' );
			GL_VERIFY(glGetShaderInfoLog, shader_id, size, &size, msg.data());
			msg.resize(size);
			return { msg };
		}
		return std::nullopt;
	}

	static std::string stageToString(ShaderSource::Stage stage)
	{
		switch (stage)
		{
		case ShaderSource::Stage::Vertex: return "Vertex";
		case ShaderSource::Stage::Fragment: return "Fragment";
		}
		std::unreachable();
	}

	static GLenum stageToGL(ShaderSource::Stage stage)
	{
		switch (stage)
		{
		case ShaderSource::Stage::Vertex: return GL_VERTEX_SHADER;
		case ShaderSource::Stage::Fragment: return GL_FRAGMENT_SHADER;
		}
		std::unreachable();
	}

	Shader::Shader(const ShaderSource& source) :
		m_id(0),
		m_stage(source.getStage())
	{
		m_id = GL_VERIFY(glCreateShader, stageToGL(m_stage));

		const std::string_view src = source.getSource();
		const GLchar* c_source = src.data();
		const GLint c_size = src.size();
		GL_VERIFY(glShaderSource, m_id, 1, &c_source, &c_size);
		GL_VERIFY(glCompileShader, m_id);

		if(auto compilation_status = checkShaderCompilationStatus(m_id); compilation_status.has_value())
		{
			destroy();
			core::log::fatal("Failed to compile {} shader. {}", stageToString(m_stage), compilation_status.value());
			throw ShaderException(std::format("Failed to compile {} shader. {}", stageToString(m_stage), compilation_status.value()));
		}

		core::log::trace("Shader {} {} created", stageToString(m_stage), m_id);
	}

	void Shader::destroy() noexcept
	{
		if(m_id != 0)
		{
			GL_VERIFY(glDeleteShader, m_id);
			core::log::trace("Shader {} {} destroyed", stageToString(m_stage), m_id);
			m_id = 0;
		}
	}

	Shader::~Shader()
	{
		destroy();
	}

	Shader::Shader(Shader&& oth) noexcept :
		m_id(std::exchange(oth.m_id, 0)),
		m_stage(oth.m_stage)
	{
	}

	Shader& Shader::operator=(Shader&& oth) noexcept
	{
		if (this != &oth)
		{
			destroy();
			m_id = std::exchange(oth.m_id, 0);
			m_stage = oth.m_stage;
		}
		return *this;
	}
	
	
	ShaderProgram::ShaderProgram(const Shader& vertex_shader, const Shader& fragment_shader) :
		m_id(0)
	{
		core::verify(vertex_shader.getStage() == ShaderSource::Stage::Vertex, "Incorect shader stage passed in ShaderProgram ctor");
		core::verify(fragment_shader.getStage() == ShaderSource::Stage::Fragment, "Incorect shader stage passed in ShaderProgram ctor");

		m_id = GL_VERIFY(glCreateProgram);
		GL_VERIFY(glAttachShader, m_id, vertex_shader.getId());
		GL_VERIFY(glAttachShader, m_id, fragment_shader.getId());
		GL_VERIFY(glLinkProgram, m_id);
		GL_VERIFY(glValidateProgram, m_id);

		int status;
		GL_VERIFY(glGetProgramiv, m_id, GL_LINK_STATUS, &status);
		if (status == GL_FALSE)
		{
			int length;
			GL_VERIFY(glGetProgramiv, m_id, GL_INFO_LOG_LENGTH, &length);
			std::string log(length, '\0');
			GL_VERIFY(glGetProgramInfoLog, m_id, length, &length, log.data());
			log.resize(length);
			GL_VERIFY(glDetachShader, m_id, vertex_shader.getId());
			GL_VERIFY(glDetachShader, m_id, fragment_shader.getId());
			glDeleteProgram(m_id);
			throw ShaderException(std::format("Failed to link shader program: {}", log));
		}

		if constexpr (config::is_debug)
		{
			GL_VERIFY(glGetProgramiv, m_id, GL_VALIDATE_STATUS, &status);
			if (status == GL_FALSE)
				core::log::warning("ShaderProgram {} validation failed. Draw command can generate error!", m_id);
		}

		GL_VERIFY(glDetachShader, m_id, vertex_shader.getId());
		GL_VERIFY(glDetachShader, m_id, fragment_shader.getId());

		core::log::trace("ShaderProgram {} created", m_id);
	}
	
	ShaderProgram::~ShaderProgram()
	{
		destroy();
	}
	
	ShaderProgram::ShaderProgram(ShaderProgram&& oth) noexcept :
		m_id(std::exchange(oth.m_id, 0)),
		m_uniform_cache(std::move(oth.m_uniform_cache))
	{
	}
	
	ShaderProgram& ShaderProgram::operator=(ShaderProgram&& oth) noexcept
	{
		if (this != &oth)
		{
			destroy();
			m_id = std::exchange(oth.m_id, 0);
			m_uniform_cache = std::move(oth.m_uniform_cache);
		}
		return *this;
	}
	
	void ShaderProgram::destroy() noexcept
	{
		if(m_id != 0)
		{
			core::log::trace("ShaderProgram {} destroyed!", m_id);
			GL_VERIFY(glDeleteProgram, m_id);
			m_id = 0;
		}
	}
	
	void ShaderProgram::bind() const
	{
		core::verify(m_id != 0, "Trying to bind invalid ShaderProgram");
		GL_VERIFY(glUseProgram, m_id);
	}
	
	void ShaderProgram::unbind()
	{
		GL_VERIFY(glUseProgram, 0);
	}
	
	void ShaderProgram::setUniform(std::string_view name, float f0, float f1, float f2, float f3)
	{
		GL_VERIFY(glUniform4f, getUniformLocation(name), f0, f1, f2, f3);
	}
	
	void ShaderProgram::setUniform(std::string_view name, float f0, float f1, float f2)
	{
		GL_VERIFY(glUniform3f, getUniformLocation(name), f0, f1, f2);
	}
	
	void ShaderProgram::setUniform(std::string_view name, float f)
	{
		GL_VERIFY(glUniform1f, getUniformLocation(name), f);
	}
	
	void ShaderProgram::setUniform(std::string_view name, int i)
	{
		GL_VERIFY(glUniform1i, getUniformLocation(name), i);
	}
	
	void ShaderProgram::setUniform(std::string_view name, const glm::mat4& mat)
	{
		GL_VERIFY(glUniformMatrix4fv, getUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
	}
	
	void ShaderProgram::setUniform(std::string_view name, const glm::vec3& vec)
	{
		GL_VERIFY(glUniform3fv, getUniformLocation(name), 1, glm::value_ptr(vec));
	}
	
	void ShaderProgram::setUniform(std::string_view name, const glm::vec4& vec)
	{
		GL_VERIFY(glUniform4fv, getUniformLocation(name), 1, glm::value_ptr(vec));
	}
	
	void ShaderProgram::setUniform(std::string_view name, const int* values, std::size_t count)
	{
		GL_VERIFY(glUniform1iv, getUniformLocation(name), static_cast<int>(count), values);
	}
	
	void ShaderProgram::setUniform(std::string_view name, bool b)
	{
		GL_VERIFY(glUniform1i, getUniformLocation(name), b ? 1 : 0);
	}
	
	int ShaderProgram::getUniformLocation(std::string_view name)
	{
		std::string key{ name };

		if (m_uniform_cache.contains(key))
			return m_uniform_cache[key];

		int location = GL_VERIFY(glGetUniformLocation, m_id, key.c_str());
		if (location == -1)
		{
			core::log::warning("Uniform \"{0}\" doesn`t exist!", name);
			return -1;
		}
		m_uniform_cache.emplace(std::move(key), location);

		return location;
	}

}

