module bm.res:shader;
import :shader;

import std;

namespace bm::res
{

	constexpr std::string_view basic_vertex =
		R"(
			#version 330 core

			layout(location = 0) in vec3 vertex_coord;
			layout(location = 1) in vec2 texture_coord;
			layout(location = 2) in vec3 normal_coord;
			
			uniform mat4 u_model;
			uniform mat4 u_view;
			uniform mat4 u_projection;
			
			out vec2 v_uv;
			out vec3 v_worldPos;
			out vec3 v_normal;
			
			void main()
			{
			    vec4 worldPos = u_model * vec4(vertex_coord, 1.0);
			    v_worldPos = worldPos.xyz;
			
			    mat3 normalMatrix = mat3(transpose(inverse(u_model)));
			    v_normal = normalize(normalMatrix * normal_coord);
			
			    v_uv = texture_coord;
			    gl_Position = u_projection * u_view * worldPos;
			}
		)";

	constexpr std::string_view basic_fragment =
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
			uniform sampler2D u_normal_map;
			uniform bool u_has_normal_map;
			
			uniform vec3 u_light_color;
			uniform vec3 u_light_pos;
			uniform vec3 u_view_pos;
			
			in vec2 v_uv;
			in vec3 v_worldPos;
			in vec3 v_normal;
			
			vec3 getNormal()
			{
			    vec3 N = normalize(v_normal);
			
			    if (!u_has_normal_map)
			        return N;
			
			    // Derivative-based TBN (robust, no tangents needed)
			    vec3 dp1 = dFdx(v_worldPos);
			    vec3 dp2 = dFdy(v_worldPos);
			    vec2 duv1 = dFdx(v_uv);
			    vec2 duv2 = dFdy(v_uv);
			
			    vec3 T = normalize(dp1 * duv2.y - dp2 * duv1.y);
			    vec3 B = normalize(-dp1 * duv2.x + dp2 * duv1.x);
			
			    mat3 TBN = mat3(T, B, N);
			
			    vec3 n = texture(u_normal_map, v_uv).rgb;
			    n = n * 2.0 - 1.0;
			
			    return normalize(TBN * n);
			}
			
			void main()
			{
			    vec3 N = getNormal();
			    vec3 L = normalize(u_light_pos - v_worldPos);
			    vec3 V = normalize(u_view_pos - v_worldPos);
			    vec3 R = reflect(-L, N);
			
			    float diff = max(dot(N, L), 0.0);
			    float spec = pow(max(dot(V, R), 0.0), max(u_material.shininess, 1.0));
			
			    vec3 ambient  = u_material.ambient * u_light_color;
			    vec3 diffuse  = diff * u_material.diffuse * u_light_color;
			    vec3 specular = spec * u_material.specular * u_light_color;
			
			    vec4 texColor = texture(u_sampler2d, v_uv);
			    vec3 baseColor = texColor.rgb * u_material.color.rgb;
			
			    vec3 colorRGB = baseColor * (ambient + diffuse) + specular;
			    float alpha = texColor.a * u_material.color.a;
			
			    o_fragment = vec4(colorRGB, alpha);
			}
		)";
}