module;

#include "glm/mat4x4.hpp"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"

export module bm.gfx.shader;

import bm.traits;

import std;

namespace bm::gfx
{

export class Shader
{
private:

	struct ProgramSource;
	struct Cache;

public:

	enum class Type
	{
		Float,
		Float2,
		Float3,
		Float4,
		Float2x2,
		Float3x3,
		Float4x4,

		Int,
		Int2,
		Int3,
		Int4,

		UInt,
		UInt2,
		UInt3,
		UInt4,

		Bool,
	};

public:


	Shader(const std::filesystem::path& filepath);
	Shader() : Shader(basic_vertex, basic_fragment) {}
	explicit Shader(std::string_view vertex, std::string_view fragment);

	~Shader();

	Shader(const Shader&) = delete;
	Shader& operator=(const Shader&) = delete;
	Shader(Shader&& oth) noexcept;
	Shader& operator=(Shader&& oth) noexcept;


	void bind() const;
	void unbind() const;
	void destroy();

	unsigned int getID() const { return m_id; }

	void setUniform(std::string_view name, float f0, float f1, float f2, float f3);
	void setUniform(std::string_view name, float f0, float f1, float f2);
	void setUniform(std::string_view name, float f);
	void setUniform(std::string_view name, int i);
	void setUniform(std::string_view name, const glm::vec3& vec);
	void setUniform(std::string_view name, const glm::vec4& vec);
	void setUniform(std::string_view name, const glm::mat4& mat);
	void setUniform(std::string_view name, const int* values, std::size_t count);

	template<typename... Args>
	static Traits<Shader>::SPtr make(Args&&... args) { return std::make_shared<Shader>(std::forward<Args>(args)...); }

private:

	int getUniformLocation(std::string_view);
	unsigned int compileShader(unsigned int, std::string_view);
	ProgramSource parseShader(std::istream& stream);
	ProgramSource parseFromFile(const std::filesystem::path& path);
	ProgramSource parseFromString(std::string_view src);
	unsigned int createProgram(std::string_view, std::string_view);

public:

	static constexpr std::string_view basic_vertex = 
	R"(
        #version 330 core
        layout(location = 0) in vec3 vertex_coord;
        layout(location = 1) in vec2 texture_coord;
		layout(location = 2) in vec3 normal_coord;

        uniform mat4 u_model;
        uniform mat4 u_view;
        uniform mat4 u_projection;

        out vec2 f_texture_coord;
		out vec3 f_normal_coord;
		out vec3 f_frag_pos;

        void main()
        {
            f_texture_coord = texture_coord;
			f_normal_coord = normal_coord;
			f_frag_pos = vec3(u_model * vec4(vertex_coord, 1.0));

            gl_Position = u_projection * u_view * u_model * vec4(vertex_coord, 1);
        }
	)";

	static constexpr std::string_view basic_fragment =
		R"(
		#version 330 core
		
		out vec4 o_fragment;
		
		struct Material
		{
		    vec4 color;        // base color multiplier (RGBA)
		    vec3 ambient;
		    vec3 diffuse;
		    vec3 specular;
		    float shininess;   // 1–128
		};
		
		uniform Material u_material;
		
		uniform sampler2D u_sampler2d;
		
		uniform vec3 u_light_color;
		uniform vec3 u_light_pos;
		uniform vec3 u_view_pos;
		
		in vec2 f_texture_coord;
		in vec3 f_normal_coord;
		in vec3 f_frag_pos;
		
		void main()
		{
		    // ----------------------------------------------------
		    // NORMAL & LIGHT DIRECTION
		    // ----------------------------------------------------
		    vec3 norm = normalize(f_normal_coord);
		    vec3 lightDir = normalize(u_light_pos - f_frag_pos);
		
		    // ----------------------------------------------------
		    // AMBIENT LIGHT
		    // ----------------------------------------------------
		    vec3 ambient = u_material.ambient * u_light_color;
		
		    // ----------------------------------------------------
		    // DIFFUSE LIGHT
		    // ----------------------------------------------------
		    float diff = max(dot(norm, lightDir), 0.0);
		    vec3 diffuse = diff * u_material.diffuse * u_light_color;
		
		    // ----------------------------------------------------
		    // SPECULAR LIGHT
		    // ----------------------------------------------------
		    vec3 viewDir = normalize(u_view_pos - f_frag_pos);
		    vec3 reflectDir = reflect(-lightDir, norm);
		
		    float spec = pow(max(dot(viewDir, reflectDir), 0.0),
		                     max(u_material.shininess, 1.0));
		
		    vec3 specular = spec * u_material.specular * u_light_color;
		
		    // ----------------------------------------------------
		    // FINAL LIGHT
		    // ----------------------------------------------------
		    vec3 lightResult = ambient + diffuse + specular;
		
		    // ----------------------------------------------------
		    // TEXTURE SAMPLE
		    // ----------------------------------------------------
		    vec4 texColor = texture(u_sampler2d, f_texture_coord);
		
		    // ----------------------------------------------------
		    // FINAL OUTPUT COLOR
		    // ----------------------------------------------------
		    vec3 colorRGB = texColor.rgb * u_material.color.rgb * lightResult;
		    float alpha = texColor.a * u_material.color.a;
		
		    o_fragment = vec4(colorRGB, alpha);
		}

		)";



private:

	unsigned int m_id = 0;
	std::filesystem::path m_filepath;
	std::unique_ptr<Cache> m_cache;
};

}

