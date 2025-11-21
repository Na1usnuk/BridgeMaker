#shader vertex

#version 450 core
layout (location = 0) in vec3 vertex_coord;
layout (location = 1) in vec2 texture_coord;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

out vec2 f_texture_coord;

void main()
{
    f_texture_coord = texture_coord;

    gl_Position = u_projection * u_view * u_model * vec4(vertices, 1);
}


#shader fragment


#version 450 core
out vec4 o_fragment;

uniform sampler2D u_sampler2d;
uniform vec4 u_color;

in vec2 f_texture_coord;

void main()
{
    o_fragment = u_color * texture(u_sampler2d, f_texture_coord);
}
