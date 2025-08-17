#include "pch.hpp"

#include "opengl/shader.hpp"
#include "opengl/opengl.hpp"

#include <fstream>
#include <sstream>



BM_START
GL_START

struct shaderProgramSource
{
	std::string vertexSource;
	std::string fragmentSource;
};

Shader::Shader(std::string_view filepath)
    :  m_filepath(filepath)
{
	shaderProgramSource src = parseShader(filepath);
	m_id = createProgram(src.vertexSource, src.fragmentSource);
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
	m_uniform_cache = std::move(oth.m_uniform_cache);
	oth.m_id = 0;
}

Shader& Shader::operator=(Shader&& oth) noexcept
{
	if (m_id == oth.m_id)
		return *this;
	m_id = oth.m_id;
	m_filepath = std::move(oth.m_filepath);
	m_uniform_cache = std::move(oth.m_uniform_cache);
	oth.m_id = 0;
	return *this;
}

void Shader::destroy()
{
	GLCALL(glDeleteProgram(m_id));
	BM_CORE_TRACE("Shader {0} was deleted!", m_id);
}

void Shader::bind() const
{
	GLCALL(glUseProgram(m_id));
}

void Shader::unbind() const
{
	GLCALL(glUseProgram(0));
}

void Shader::setUniform(std::string_view name, float f0, float f1, float f2, float f3)
{
	bind();
	GLCALL(glUniform4f(getUniformLocation(name), f0, f1, f2, f3));
}

void Shader::setUniform(std::string_view name, float f0, float f1, float f2)
{
	bind();
	GLCALL(glUniform3f(getUniformLocation(name), f0, f1, f2));
}

void Shader::setUniform(std::string_view name, float f)
{
	bind();
	GLCALL(glUniform1f(getUniformLocation(name), f));
}

void Shader::setUniform(std::string_view name, int i)
{
	bind();
	GLCALL(glUniform1i(getUniformLocation(name), i));
}

void Shader::setUniform(std::string_view name, const glm::mat4& mat)
{
	bind();
	GLCALL(glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &mat[0][0]));
}

int Shader::getUniformLocation(std::string_view name)
{
	if (m_uniform_cache.contains(name.data()))
		return m_uniform_cache[name.data()];
	else
	{
		GLCALL(int location = glGetUniformLocation(m_id, name.data()));
		if (location == -1)
		{
			BM_CORE_WARNING("Uniform \"{0}\" doesn`t exist!", name);
			return -1;
		}
		m_uniform_cache[name.data()] = location;
		return location;
	}
}

unsigned int Shader::compileShader(unsigned int type, std::string_view source)
{
	GLCALL(unsigned int id = glCreateShader(type));
	const char* src = source.data();
	GLCALL(glShaderSource(id, 1, &src, nullptr));
	GLCALL(glCompileShader(id));

	int result;
	GLCALL(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
	if (result == GL_FALSE)
	{
		int l;
		GLCALL(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &l));
		char* msg = (char*)alloca(l * sizeof(char));
		GLCALL(glGetShaderInfoLog(id, l, &l, msg));
		BM_CORE_ERROR("Failed to compile {0} shader. {1}", (type == GL_VERTEX_SHADER ? "vertex" : "fragment"), msg);
		GLCALL(glDeleteShader(id));
		return 0;
	}
	return id;
}

unsigned int Shader::createProgram(std::string_view vertexShader, std::string_view fragmentShader)
{
	GLCALL(unsigned int prog = glCreateProgram());
	unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

	GLCALL(glAttachShader(prog, vs));
	GLCALL(glAttachShader(prog, fs));
	GLCALL(glLinkProgram(prog));
	GLCALL(glValidateProgram(prog));

	GLCALL(glDeleteShader(vs));
	GLCALL(glDeleteShader(fs));

	return prog;
}

shaderProgramSource Shader::parseShader(std::string_view filepath)
{
	std::ifstream stream(filepath.data());

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
	BM_CORE_ASSERT(!(ss[0].str().empty() || ss[1].str().empty()), "Shaders are empty");
	return { ss[0].str(), ss[1].str() };
}

GL_END
BM_END
