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

export  constexpr const char* const basic_vertex_shader = R"(
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

export  constexpr const char* const basic_fragment_shader = R"(
            #version 330 core
            out vec4 o_fragment;
            uniform sampler2D samp;
            void main()
            {
                o_fragment = vec4(1,1,1,1);
            }
        )";


export class Triangle : public Object
{
public:

    Triangle()
    {
        static auto vbo = VertexBuffer::make({
                -0.5f, -0.5f, 0.0f,
                0.5f, -0.5f, 0.0f,
                0.0f, 0.5f, 0.0f,
            });
        vbo->pushLayout<float>(3);

        static auto ibo = IndexBuffer::make({
                0, 1, 2,
            });

        static auto vao = VertexArray::make(vbo);

        static auto shader = Shader::make(basic_vertex_shader, basic_fragment_shader);

        setShader(shader);
        setVertexArray(vao);
        setIndexBuffer(ibo);
    }
};


export class Square : public Object
{
public:

    Square()
	{
        static float vertices[] =
        {
            -0.5, 0.5f, 0.0f,
            0.5f, 0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            -0.5f, -0.5f, 0.0f
        };

        static unsigned int indices[] =
        {
            0, 1, 2,
            2, 3, 0
        };

        static auto vbo = VertexBuffer::make(vertices, sizeof(vertices));
        vbo->pushLayout<float>(3);
        static auto ibo = IndexBuffer::make(indices, sizeof(indices));
        static auto vao = VertexArray::make(vbo);
        static auto shader = Shader::make(basic_vertex_shader, basic_fragment_shader);

        setShader(shader);
        setVertexArray(vao);
        setIndexBuffer(ibo);
	}

private:
};


export class Cube : public Object
{

public:

    Cube()
    {
        static float vertices[] = 
        {
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
             0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,

            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,

            -0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

             0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

             -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
              0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
              0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
             -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
                                             
             -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
              0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
              0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
             -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        };

        static unsigned int indices[] = 
        {
            0, 1, 2,
            2, 3, 0,

            4, 5, 6,
            6, 7, 4,

            8, 9,10,
            10,11, 8,

            12,13,14,
            14,15,12,

            16,17,18,
            18,19,16,

            20,21,22,
            22,23,20
        };

        static auto vbo = VertexBuffer::make(vertices, sizeof(vertices));
        vbo->pushLayout<float>(3); //for vertices
        vbo->pushLayout<float>(2); //for texture

        static auto ibo = IndexBuffer::make(indices, sizeof(indices));

        static auto vao = VertexArray::make(vbo);

        static auto shader = Shader::make(basic_vertex_shader, basic_fragment_shader);

        setShader(shader);
        setVertexArray(vao);
        setIndexBuffer(ibo);
    }
};




}