module;

#include "glm/mat4x4.hpp"
#include "glm/vec3.hpp"

export module bm.gfx.shader;

import std;

namespace bm::gfx
{


export class Shader
{
private:

	struct ProgramSource;
	struct Cache;

public:

	using Ptr = std::shared_ptr<Shader>;
	using KPtrRef = const Ptr&;

public:

	Shader(const std::filesystem::path& filepath);
	Shader(std::string_view src);
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
	void setUniform(std::string_view name, const glm::vec3& vec);
	void setUniform(std::string_view name, const glm::mat4& mat);

	template<typename... Args>
	static Ptr make(Args&&... args) { return std::make_shared<Shader>(std::forward<Args>(args)...); }


private:

	int getUniformLocation(std::string_view);
	unsigned int compileShader(unsigned int, std::string_view);
	ProgramSource parseShader(std::istream& stream);
	ProgramSource parseFromFile(const std::filesystem::path& path);
	ProgramSource parseFromString(std::string_view src);
	unsigned int createProgram(std::string_view, std::string_view);

private:

	unsigned int m_id = 0;
	std::filesystem::path m_filepath;
	std::unique_ptr<Cache> m_cache;
};

export using ShaderPtr = Shader::Ptr;

}

