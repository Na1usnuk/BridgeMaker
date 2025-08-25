module;

#include "glm/mat4x4.hpp"

export module bm.gfx.shader;

import std;

namespace bm::gfx
{

struct shaderProgramSource;

export class Shader
{
public:

	Shader(std::string_view);
	~Shader();

	Shader(const Shader&) = delete;
	Shader& operator=(const Shader&) = delete;
	Shader(Shader&& oth) noexcept;
	Shader& operator=(Shader&& oth) noexcept;


	void bind() const;
	void unbind() const;
	void destroy();

	void setUniform(std::string_view name, float f0, float f1, float f2, float f3);
	void setUniform(std::string_view name, float f0, float f1, float f2);
	void setUniform(std::string_view name, float f);
	void setUniform(std::string_view name, int i);
	void setUniform(std::string_view name, const glm::mat4& mat);


private:

	int getUniformLocation(std::string_view);
	unsigned int compileShader(unsigned int, std::string_view);
	shaderProgramSource parseShader(std::string_view);
	unsigned int createProgram(std::string_view, std::string_view);

private:

	unsigned int m_id = 0;
	std::string m_filepath;
	std::unordered_map<std::string, int> m_uniform_cache;
};

}

