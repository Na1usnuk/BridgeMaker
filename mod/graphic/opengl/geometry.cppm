export module bm.gfx.geometry;

import bm.gfx.mesh;
import bm.gfx.buffer.vertex;
import bm.gfx.buffer.index;
import bm.gfx.vertexarray;
import bm.gfx.utility;
import bm.gfx.obj;

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


export class Cube : public Object
{

public:

    Cube();

private:


};


Cube::Cube()
{
    static float vertices[] = {
        // Back face
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  // 0
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  // 1
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  // 2
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  // 3

        // Front face
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  // 4
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  // 5
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  // 6
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,  // 7

        // Left face
        -0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  // 8
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  // 9
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,  // 10
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  // 11

        // Right face
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  // 12
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  // 13
         0.5f,  0.5f,  0.5f,  0.0f, 1.0f,  // 14
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  // 15

         // Bottom face
         -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  // 16
          0.5f, -0.5f, -0.5f,  1.0f, 1.0f,  // 17
          0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  // 18
         -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  // 19

         // Top face
         -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  // 20
          0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  // 21
          0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  // 22
         -0.5f,  0.5f,  0.5f,  0.0f, 0.0f   // 23
    };

    static unsigned int indices[] = {
        // Back face
        0, 1, 2,
        2, 3, 0,

        // Front face
        4, 5, 6,
        6, 7, 4,

        // Left face
        8, 9,10,
        10,11, 8,

        // Right face
        12,13,14,
        14,15,12,

        // Bottom face
        16,17,18,
        18,19,16,

        // Top face
        20,21,22,
        22,23,20
    };



    static auto vbo = VertexBuffer::make(vertices, sizeof(vertices));
    vbo->pushLayout<float>(3); //for vertices
    vbo->pushLayout<float>(2); //for texture

    static auto ibo = IndexBuffer::make(indices, sizeof(indices));

    static auto vao = VertexArray::make(vbo);

    static const std::string shader_vertex_src = R"(
            #version 330 core
            layout (location = 0) in vec3 vertices;
            uniform mat4 u_model;
            uniform mat4 u_view;
            uniform mat4 u_projection;
            void main()
            {
                gl_Position = u_projection * u_view * u_model * vec4(vertices, 1);
            }
        )";

    static const std::string shader_fragment_src = R"(
            #version 330 core
            out vec4 o_fragment;
            uniform sampler2D samp;
            void main()
            {
                o_fragment = vec4(1,1,1,1);
            }
        )";

    static auto shader = Shader::make(shader_vertex_src, shader_fragment_src);

    setShader(shader);
    setVertexArray(vao);
    setIndexBuffer(ibo);
}

}