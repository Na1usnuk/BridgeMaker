export module bm.gfx.geometry;

import bm.gfx.mesh;
import bm.gfx.buffer.vertex;
import bm.gfx.buffer.index;

import std;


namespace bm::gfx
{

export  constexpr const char* const basic_vertex_shader = ""
"#version 330 core"
"layout (location = 0) in vec3 vertices;"
"void main() { gl_Position = vec4(vertices, 1.0); }";

export  constexpr const char* const basic_fragment_shader = ""
"#version 330 core"
"out vec4 color;"
"void main() { color = vec4(1.0, 1.0, 1.0, 1.0); }";


export class Triangle : public Mesh
{
public:

	Triangle(const std::shared_ptr<VertexArray>& vao)
		: Mesh(vao, std::make_shared<IndexBuffer>(std::initializer_list<unsigned int>{0, 1, 2}), std::make_shared<Shader>(basic_vertex_shader, basic_fragment_shader))
	{
	}

private:
};


export class Squad : public Mesh
{
public:

	Squad(const std::shared_ptr<VertexArray>& vao)
		: Mesh(vao, std::make_shared<IndexBuffer>(std::initializer_list<unsigned int>{ 0, 1, 2, 2, 3, 0 }), std::make_shared<Shader>(basic_vertex_shader, basic_fragment_shader))
	{
	}

private:
};

}