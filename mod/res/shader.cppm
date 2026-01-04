export module bm.res:shader;

import std;

namespace bm::res
{
	export constexpr std::string_view basic_vertex_shader =
		R"(
			#version 330 core

			layout(location = 0) in vec3 vertex_coord;
			
			uniform mat4 u_view_projection;
			uniform mat4 u_model;
			
			void main()
			{
				gl_Position = u_model * u_view_projection * vec4(vertex_coord, 1);
			}
		)";

	export constexpr std::string_view basic_fragment_shader =
		R"(
			#version 330 core

			out vec4 o_color;
			
			void main()
			{
			    o_color = vec4(1, 0, 0, 1);
			}
		)";
}