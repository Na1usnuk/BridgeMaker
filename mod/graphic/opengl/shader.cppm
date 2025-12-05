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
	void setUniform(std::string_view name, bool b);
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
	layout(location = 3) in vec3 tangent_coord;  // NEW
	
	uniform mat4 u_model;
	uniform mat4 u_view;
	uniform mat4 u_projection;
	
	out vec2 f_texture_coord;
	out vec3 f_frag_pos;         // world-space position
	out mat3 f_TBN;              // TBN matrix for normal mapping
	
	void main()
	{
	    f_texture_coord = texture_coord;
	
	    // world-space position
	    vec4 worldPos = u_model * vec4(vertex_coord, 1.0);
	    f_frag_pos = worldPos.xyz;
	
	    // world-space normal & tangent
	    mat3 normalMatrix = mat3(transpose(inverse(u_model)));
	    vec3 N = normalize(normalMatrix * normal_coord);
	    vec3 T = normalize(normalMatrix * tangent_coord);
	    vec3 B = normalize(cross(N, T));   // bitangent
	
	    f_TBN = mat3(T, B, N);
	
	    gl_Position = u_projection * u_view * worldPos;
	}

	)";

	static constexpr std::string_view basic_fragment =
		R"(
		#version 330 core

out vec4 o_fragment;

struct Material
{
    vec4 color;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform Material u_material;

uniform sampler2D u_sampler2d;
uniform sampler2D u_normal_map;   // NEW

uniform bool u_has_normal_map;    // NEW

uniform vec3 u_light_color;
uniform vec3 u_light_pos;
uniform vec3 u_view_pos;

in vec2 f_texture_coord;
in vec3 f_frag_pos;
in mat3 f_TBN;                    // TBN from vertex

void main()
{
    // get normal
    vec3 N;
    if (u_has_normal_map)
    {
        // sample normal map in tangent space [0..1] -> [-1..1]
        vec3 n_tangent = texture(u_normal_map, f_texture_coord).rgb;
        n_tangent = n_tangent * 2.0 - 1.0;
        N = normalize(f_TBN * n_tangent);  // to world space
    }
    else
    {
        // fallback: use geometric normal (TBN[2] is world-space N)
        N = normalize(f_TBN[2]);
    }

    vec3 L = normalize(u_light_pos - f_frag_pos);
    vec3 V = normalize(u_view_pos - f_frag_pos);
    vec3 R = reflect(-L, N);

    float diff = max(dot(N, L), 0.0);
    float spec = pow(max(dot(V, R), 0.0), max(u_material.shininess, 1.0));

    vec3 ambient  = u_material.ambient * u_light_color;
    vec3 diffuse  = diff * u_material.diffuse * u_light_color;
    vec3 specular = spec * u_material.specular * u_light_color;

    vec4 texColor = texture(u_sampler2d, f_texture_coord);
    vec3 baseColor = texColor.rgb * u_material.color.rgb;

    vec3 colorRGB = baseColor * (ambient + diffuse) + specular;
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

