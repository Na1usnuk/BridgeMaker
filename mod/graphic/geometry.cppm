export module bm.gfx.geometry;

import bm.gfx.mesh;
import bm.gfx.buffer.vertex;
import bm.gfx.buffer.index;
import bm.gfx.vertexarray;
import bm.gfx.utility;

import std;


namespace bm::gfx
{

export  constexpr const char* const basic_vertex_shader = ""
"#version 330 core\n"
"layout (location = 0) in vec3 vertices;\n"
"void main() { gl_Position = vec4(vertices, 1.0); }\n";

export  constexpr const char* const basic_fragment_shader = ""
"#version 330 core\n"
"out vec4 color;\n"
"void main() { color = vec4(1.0, 1.0, 1.0, 1.0); }\n";


export class Triangle : public Mesh
{
public:

	Triangle(const std::shared_ptr<VertexArray>& vao)
		: Mesh(vao, std::make_shared<IndexBuffer>(std::initializer_list<unsigned int>{0, 1, 2}), std::make_shared<Shader>(basic_vertex_shader, basic_fragment_shader))
	{
	}

	Triangle(const std::shared_ptr<VertexBuffer>& vbo)
	{
		//auto ibo = std::make_shared<IndexBuffer>(std::array<unsigned int, 3>{ 0,1,2 });
		//auto shader = std::make_shared<Shader>(basic_vertex_shader, basic_fragment_shader);
		//auto layout = std::make_shared<VertexBufferLayout>();
		//layout->push<float>(3);
		//VertexArrayPtr vao = std::make_shared<VertexArray>(vbo);
		//setVertexArray(vao);
		//setShader(shader);
		//setIndexBuffer(ibo);
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