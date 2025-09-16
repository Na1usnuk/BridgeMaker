module;

#include "glad/glad.h"
#include "glm/mat4x4.hpp"
#include <glm/gtc/type_ptr.hpp>

module bm.gfx.shader;


import bm.gfx.utility;
import bm.log;
import bm.assert;

namespace bm::gfx
{


struct Shader::Cache
{
		std::unordered_map<std::string, int> uniform;
};

struct Shader::ProgramSource
{
	std::string vertexSource;
	std::string fragmentSource;
};

Shader::Shader(const std::filesystem::path& filepath)
    :  m_filepath(filepath), m_cache(std::make_unique<Cache>())
{
	core::verify(std::filesystem::exists(filepath), "File " + filepath.string() + " do not exist!");
	ProgramSource src = parseFromFile(filepath);
	m_id = createProgram(src.vertexSource, src.fragmentSource);
}

Shader::Shader(std::string_view src)
	: m_cache(std::make_unique<Cache>())
{
	ProgramSource srcs = parseFromString(src);
	m_id = createProgram(srcs.vertexSource, srcs.fragmentSource);
}


Shader::~Shader()
{
	destroy();
}

Shader::Shader(Shader&& oth) noexcept
{
	if (m_id == oth.m_id)
		return;
	m_id = oth.m_id;
	m_filepath = std::move(oth.m_filepath);
	m_cache = std::move(oth.m_cache);
	oth.m_id = 0;
}

Shader& Shader::operator=(Shader&& oth) noexcept
{
	if (m_id == oth.m_id)
		return *this;
	m_id = oth.m_id;
	m_filepath = std::move(oth.m_filepath);
	m_cache = std::move(oth.m_cache);
	oth.m_id = 0;
	return *this;
}

void Shader::destroy()
{
	glCall(glDeleteProgram, m_id);
	log::core::trace("Shader {0} was deleted!", m_id);
}

void Shader::bind() const
{
	glCall(glUseProgram, m_id);
}

void Shader::unbind() const
{
	glCall(glUseProgram, 0);
}

void Shader::setUniform(std::string_view name, float f0, float f1, float f2, float f3)
{
	bind();
	glCall(glUniform4f, getUniformLocation(name), f0, f1, f2, f3);
}

void Shader::setUniform(std::string_view name, float f0, float f1, float f2)
{
	bind();
	glCall(glUniform3f, getUniformLocation(name), f0, f1, f2);
}

void Shader::setUniform(std::string_view name, float f)
{
	bind();
	glCall(glUniform1f, getUniformLocation(name), f);
}

void Shader::setUniform(std::string_view name, int i)
{
	bind();
	glCall(glUniform1i, getUniformLocation(name), i);
}

void Shader::setUniform(std::string_view name, const glm::mat4& mat)
{
	bind();
	glCall(glUniformMatrix4fv, getUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setUniform(std::string_view name, const glm::vec3& vec)
{
	bind();
	glCall(glUniform3fv, getUniformLocation(name), 1, glm::value_ptr(vec));
}

int Shader::getUniformLocation(std::string_view name)
{
	if (m_cache->uniform.contains(name.data()))
		return m_cache->uniform[name.data()];
	else
	{
		int location = glCall(glGetUniformLocation, m_id, name.data());
		if (location == -1)
		{
			log::core::warning("Uniform \"{0}\" doesn`t exist!", name);
			return -1;
		}
		m_cache->uniform[name.data()] = location;
		return location;
	}
}

unsigned int Shader::compileShader(unsigned int type, std::string_view source)
{
	unsigned int id = glCall(glCreateShader, type);
	const char* src = source.data();
	glCall(glShaderSource, id, 1, &src, nullptr);
	glCall(glCompileShader, id);

	int result;
	glCall(glGetShaderiv, id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int l;
		glCall(glGetShaderiv, id, GL_INFO_LOG_LENGTH, &l);
		char* msg = (char*)alloca(l * sizeof(char));
		glCall(glGetShaderInfoLog, id, l, &l, msg);
		log::core::error("Failed to compile {0} shader. {1}", (type == GL_VERTEX_SHADER ? "vertex" : "fragment"), msg);
		glCall(glDeleteShader, id);
		return 0;
	}
	return id;
}

unsigned int Shader::createProgram(std::string_view vertexShader, std::string_view fragmentShader)
{
	unsigned int prog = glCall(glCreateProgram);
	unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glCall(glAttachShader, prog, vs);
	glCall(glAttachShader, prog, fs);
	glCall(glLinkProgram, prog);
	glCall(glValidateProgram, prog);

	glCall(glDeleteShader, vs);
	glCall(glDeleteShader, fs);

	return prog;
}

Shader::ProgramSource Shader::parseShader(std::istream& stream)
{
	enum class shaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	shaderType type = shaderType::NONE;
	std::stringstream ss[2];
	std::string line;
	while (std::getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
				type = shaderType::VERTEX;
			else if (line.find("fragment") != std::string::npos)
				type = shaderType::FRAGMENT;
		}
		else
		{
			ss[(int)type] << line << '\n';
		}
	}
	core::verify(!(ss[0].str().empty() || ss[1].str().empty()), "Shaders are empty");
	return { ss[0].str(), ss[1].str() };
}

Shader::ProgramSource Shader::parseFromFile(const std::filesystem::path& filepath)
{
	std::ifstream file(filepath);
	core::verify(file.is_open(), "Failed to open shader file: " + filepath.string());
	return parseShader(file); // OK: file is a named lvalue
}

Shader::ProgramSource Shader::parseFromString(std::string_view str)
{
	std::string full_copy_that_needs_to_be_optimized_later(str);
	std::stringstream ss(full_copy_that_needs_to_be_optimized_later);
	return parseShader(ss);
}

}

