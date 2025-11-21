export module bm.gfx.primitives;

import bm.gfx.buffer.vertex;
import bm.gfx.buffer.index;
import bm.gfx.vertexarray;
import bm.gfx.shader;
import bm.gfx.mesh;
import bm.gfx.material;
import bm.gfx.obj;

import bm.assetmanager;

import std;


namespace bm::gfx
{

export class Triangle : public Object
{
public:

    static inline const std::array<float, 24> vertices =
    {
		// positions       // texture coords    // normals
        0.0f,  0.5f, 0.0f, 0.5f, 1.0f,          0.0f, 0.0f, 1.0f,
        -0.5f,-0.5f, 0.0f, 0.0f, 0.0f,          0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f,          0.0f, 0.0f, 1.0f,
    };

    static inline const std::array<unsigned int, 3> indices =
    {
        0, 1, 2
	};

public:

    Triangle()
    {
        auto vbo = VertexBuffer::make(vertices);
        vbo->setLayout
        ({
            {Shader::Type::Float3, "a_pos"},
            {Shader::Type::Float2, "a_tex"},
            {Shader::Type::Float3, "a_norm"}
        });
        auto ibo = IndexBuffer::make(indices);
        auto vao = VertexArray::make(std::move(vbo), std::move(ibo));
        auto mesh = Mesh::make(std::move(vao), Mesh::DrawAs::Triangles);
        setMesh(mesh);
        auto material = Material::make();
        setMaterial(material);
    }
};


//export class Square : public Object
//{
//public:
//
//    Square()
//	{
//        static float vertices[] =
//        {
//            -0.5, 0.5f, 0.0f, 0.f, 1.f,
//            0.5f, 0.5f, 0.0f, 1.f, 1.f,
//            0.5f, -0.5f, 0.0f, 1.f, 0.f,
//            -0.5f, -0.5f, 0.0f, 0.f, 0.f
//        };
//
//        static unsigned int indices[] =
//        {
//            0, 1, 2,
//            2, 3, 0
//        };
//
//        static auto vbo = VertexBuffer::make(vertices, sizeof(vertices));
//        vbo->pushLayout<float>(3);
//		vbo->pushLayout<float>(2);
//        static auto ibo = IndexBuffer::make(indices, sizeof(indices));
//        static auto vao = VertexArray::make(vbo);
//
//        static auto mesh = Mesh::make(vao, ibo);
//        static auto material = Material::make();
//
//        setMaterial(material);
//        setMesh(mesh);
//	}
//
//private:
//};
//
//
//export class Cube : public Object
//{
//
//public:
//
//    Cube()
//    {
//        static float vertices[] = 
//        {
//            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
//             0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
//             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
//            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
//
//            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
//             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
//             0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
//            -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
//
//            -0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
//            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
//            -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
//            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
//
//             0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
//             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
//             0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
//             0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
//
//             -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//              0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
//              0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
//             -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
//                                             
//             -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
//              0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
//              0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//             -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
//        };
//
//        static unsigned int indices[] = 
//        {
//            0, 1, 2,
//            2, 3, 0,
//
//            4, 5, 6,
//            6, 7, 4,
//
//            8, 9,10,
//            10,11, 8,
//
//            12,13,14,
//            14,15,12,
//
//            16,17,18,
//            18,19,16,
//
//            20,21,22,
//            22,23,20
//        };
//
//        static auto vbo = VertexBuffer::make(vertices, sizeof(vertices));
//        vbo->pushLayout<float>(3); //for vertices
//        vbo->pushLayout<float>(2); //for texture
//
//        static auto ibo = IndexBuffer::make(indices, sizeof(indices));
//
//        static auto vao = VertexArray::make(vbo);
//
//        static auto mesh = Mesh::make(vao, ibo);
//
//        auto material = Material::make();
//
//        setMaterial(material);
//        setMesh(mesh);
//    }
//};
//
//
export class Grid : public Object
{
public:

    Grid(float size_of_line = 1000.f, float size_of_square_side = 0.2f)
    {
        const unsigned int lines_count = size_of_line / size_of_square_side; // Lines count in one direction

        std::vector<float> vertices;
        vertices.reserve(6 * 2 * lines_count); // 3 floats for line positions and lines count in both directions

        const float size_from_origin = size_of_line / 2.f;

        for (unsigned int i = 0; i < lines_count; ++i)
        {
            const float gap = size_from_origin - i * size_of_square_side;

            //line on x
            vertices.push_back(-size_from_origin);
            vertices.push_back(0.f);
            vertices.push_back(gap);
            vertices.push_back(size_from_origin);
            vertices.push_back(0.f);
            vertices.push_back(gap);

            //line on y
            vertices.push_back(gap);
            vertices.push_back(0.f);
            vertices.push_back(-size_from_origin);
            vertices.push_back(gap);
            vertices.push_back(0.f);
            vertices.push_back(size_from_origin);
        }

        auto vbo = VertexBuffer::make(vertices);
        vbo->setLayout({{Shader::Type::Float3, "a_pos"}});
        auto vao = VertexArray::make(std::move(vbo));

        auto mesh = Mesh::make(std::move(vao), Mesh::DrawAs::Lines);

        auto material = Material::make();
        material->setColor({ 0.f, 0.f, 0.f, 1.f });

        setMaterial(material);
        setMesh(mesh);
    }

};




}