module bm.res:primitives;
import :primitives;

import bm.gfx;
import bm.core;

import std;

namespace bm::res
{




    //Square::Square()
    //{
    //    static constexpr std::array<float, 8 * 4> vertices =
    //    {
    //        // Position          // TexCoords  // Normal
    //        -0.5f,  0.0f,  0.5f,  0.0f, 1.0f,   0.0f, 1.0f, 0.0f,
    //         0.5f,  0.0f,  0.5f,  1.0f, 1.0f,   0.0f, 1.0f, 0.0f,
    //         0.5f,  0.0f, -0.5f,  1.0f, 0.0f,   0.0f, 1.0f, 0.0f,
    //        -0.5f,  0.0f, -0.5f,  0.0f, 0.0f,   0.0f, 1.0f, 0.0f
    //    };
    //
    //    static constexpr std::array<unsigned int, 6> indices =
    //    {
    //        0, 1, 2,
    //        2, 3, 0
    //    };
    //
    //    auto vbo = VertexBuffer::make(vertices);
    //    vbo->setLayout
    //    ({
    //        {Shader::Type::Float3, "a_pos"},
    //        {Shader::Type::Float2, "a_tex"},
    //        {Shader::Type::Float3, "a_norm"}
    //    });
    //    setMesh({ {vbo, {indices}} });
    //}


    //Cube::Cube()
    //
    //

    //Grid::Grid(float size_of_line, float size_of_square_side)
    //{
    //    const unsigned int lines_count = size_of_line / size_of_square_side; // Lines count in one direction
    //
    //    std::vector<float> vertices;
    //    vertices.reserve(6 * 2 * lines_count); // 3 floats for line positions and lines count in both directions
    //
    //    const float size_from_origin = size_of_line / 2.f;
    //
    //    for (unsigned int i = 0; i < lines_count; ++i)
    //    {
    //        const float gap = size_from_origin - i * size_of_square_side;
    //
    //        //line on x
    //        vertices.push_back(-size_from_origin);
    //        vertices.push_back(0.f);
    //        vertices.push_back(gap);
    //        vertices.push_back(size_from_origin);
    //        vertices.push_back(0.f);
    //        vertices.push_back(gap);
    //
    //        //line on y
    //        vertices.push_back(gap);
    //        vertices.push_back(0.f);
    //        vertices.push_back(-size_from_origin);
    //        vertices.push_back(gap);
    //        vertices.push_back(0.f);
    //        vertices.push_back(size_from_origin);
    //    }
    //
    //    auto vbo = VertexBuffer::make(vertices);
    //    vbo->setLayout({ {Shader::Type::Float3, "a_pos"} });
    //    auto vao = VertexArray::make(std::move(vbo));
    //
    //    auto mesh = Mesh::make(std::move(vao), Mesh::DrawAs::Lines);
    //
    //    auto material = Material::make();
    //    material->setColor({ 0.f, 0.f, 0.f, 1.f });
    //
    //    setMaterial(material);
    //    setMesh(mesh);
    //}




    
    //

    //Pyramid::Pyramid()
    //{
    //    static std::array<float, 8 * 16> vertices = {
    //        // Base (bottom face, normal pointing down)
    //        -0.5f, 0.0f, -0.5f,  0.0f, 0.0f,  0.0f, -1.0f, 0.0f,
    //         0.5f, 0.0f, -0.5f,  1.0f, 0.0f,  0.0f, -1.0f, 0.0f,
    //         0.5f, 0.0f,  0.5f,  1.0f, 1.0f,  0.0f, -1.0f, 0.0f,
    //        -0.5f, 0.0f,  0.5f,  0.0f, 1.0f,  0.0f, -1.0f, 0.0f,
    //
    //        // Front face
    //        -0.5f, 0.0f,  0.5f,  0.0f, 0.0f,  0.0f, 0.447f, 0.894f,
    //         0.5f, 0.0f,  0.5f,  1.0f, 0.0f,  0.0f, 0.447f, 0.894f,
    //         0.0f, 1.0f,  0.0f,  0.5f, 1.0f,  0.0f, 0.447f, 0.894f,
    //
    //         // Right face
    //          0.5f, 0.0f,  0.5f,  0.0f, 0.0f,  0.894f, 0.447f, 0.0f,
    //          0.5f, 0.0f, -0.5f,  1.0f, 0.0f,  0.894f, 0.447f, 0.0f,
    //          0.0f, 1.0f,  0.0f,  0.5f, 1.0f,  0.894f, 0.447f, 0.0f,
    //
    //          // Back face
    //           0.5f, 0.0f, -0.5f,  0.0f, 0.0f,  0.0f, 0.447f, -0.894f,
    //          -0.5f, 0.0f, -0.5f,  1.0f, 0.0f,  0.0f, 0.447f, -0.894f,
    //           0.0f, 1.0f,  0.0f,  0.5f, 1.0f,  0.0f, 0.447f, -0.894f,
    //
    //           // Left face
    //           -0.5f, 0.0f, -0.5f,  0.0f, 0.0f,  -0.894f, 0.447f, 0.0f,
    //           -0.5f, 0.0f,  0.5f,  1.0f, 0.0f,  -0.894f, 0.447f, 0.0f,
    //            0.0f, 1.0f,  0.0f,  0.5f, 1.0f,  -0.894f, 0.447f, 0.0f,
    //    };
    //
    //    static std::array<unsigned int, 18> indices = {
    //        // Base
    //        0, 1, 2,
    //        2, 3, 0,
    //        // Faces
    //        4, 5, 6,
    //        7, 8, 9,
    //        10, 11, 12,
    //        13, 14, 15
    //    };
    //
    //    auto vbo = VertexBuffer::make(vertices);
    //    vbo->setLayout({
    //        {Shader::Type::Float3, "a_pos"},
    //        {Shader::Type::Float2, "a_tex"},
    //        {Shader::Type::Float3, "a_norm"}
    //        });
    //    auto ibo = IndexBuffer::make(indices);
    //    auto vao = VertexArray::make(std::move(vbo), std::move(ibo));
    //    static auto mesh = Mesh::make(std::move(vao));
    //    static auto material = Material::make();
    //    setMaterial(material);
    //    setMesh(mesh);
    //}
    //

    //Cone::Cone(int segments )
    //{
    //    std::vector<float> vertices;
    //    std::vector<unsigned int> indices;
    //
    //    float angleStep = 2 * 3.14159f / segments;
    //
    //    // Tip vertex
    //    vertices.insert(vertices.end(), { 0.0f, 1.0f, 0.0f, 0.5f, 1.0f, 0.0f, 1.0f, 0.0f });
    //
    //    // Base center
    //    vertices.insert(vertices.end(), { 0.0f, 0.0f, 0.0f, 0.5f, 0.0f, 0.0f, -1.0f, 0.0f });
    //
    //    // Side vertices and base vertices
    //    for (int i = 0; i <= segments; ++i) {
    //        float angle = i * angleStep;
    //        float x = 0.5f * cosf(angle);
    //        float z = 0.5f * sinf(angle);
    //
    //        // Calculate side normal (manually)
    //        // For a cone, the normal at the side points outward and slightly up
    //        // Simple approach: normal = normalize(vec3(x, 0.5, z))
    //        float nx = x;
    //        float ny = 0.5f;  // Cone slope
    //        float nz = z;
    //        float length = sqrtf(nx * nx + ny * ny + nz * nz);
    //        nx /= length;
    //        ny /= length;
    //        nz /= length;
    //
    //        // Side vertex
    //        vertices.push_back(x);
    //        vertices.push_back(0.0f);
    //        vertices.push_back(z);
    //        vertices.push_back((float)i / segments);
    //        vertices.push_back(0.0f);
    //        vertices.push_back(nx);
    //        vertices.push_back(ny);
    //        vertices.push_back(nz);
    //
    //        // Base vertex
    //        vertices.push_back(x);
    //        vertices.push_back(0.0f);
    //        vertices.push_back(z);
    //        vertices.push_back(x + 0.5f);
    //        vertices.push_back(z + 0.5f);
    //        vertices.push_back(0.0f);
    //        vertices.push_back(-1.0f);
    //        vertices.push_back(0.0f);
    //    }
    //
    //    // Side triangles
    //    for (int i = 0; i < segments; ++i) {
    //        indices.push_back(0); // Tip
    //        indices.push_back(2 + i * 2);
    //        indices.push_back(2 + (i + 1) * 2);
    //    }
    //
    //    // Base triangles
    //    for (int i = 0; i < segments; ++i) {
    //        indices.push_back(1); // Base center
    //        indices.push_back(3 + (i + 1) * 2);
    //        indices.push_back(3 + i * 2);
    //    }
    //
    //    auto vbo = VertexBuffer::make(vertices);
    //    vbo->setLayout({
    //        {Shader::Type::Float3, "a_pos"},
    //        {Shader::Type::Float2, "a_tex"},
    //        {Shader::Type::Float3, "a_norm"}
    //        });
    //    auto ibo = IndexBuffer::make(indices);
    //    auto vao = VertexArray::make(std::move(vbo), std::move(ibo));
    //    static auto mesh = Mesh::make(std::move(vao));
    //    static auto material = Material::make();
    //    setMaterial(material);
    //    setMesh(mesh);
    //}
    //
    //
    //Cylinder::Cylinder(int segments)
    //{
    //    std::vector<float> vertices;
    //    std::vector<unsigned int> indices;
    //
    //    float angleStep = 2 * 3.14159f / segments;
    //
    //    // Top center
    //    vertices.insert(vertices.end(), { 0.0f, 0.5f, 0.0f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f });
    //
    //    // Bottom center
    //    vertices.insert(vertices.end(), { 0.0f, -0.5f, 0.0f, 0.5f, 0.5f, 0.0f, -1.0f, 0.0f });
    //
    //    // Generate side vertices (duplicated for top, side, and bottom)
    //    for (int i = 0; i <= segments; ++i) {
    //        float angle = i * angleStep;
    //        float x = 0.5f * cosf(angle);
    //        float z = 0.5f * sinf(angle);
    //        float u = (float)i / segments;
    //
    //        // Top cap vertex
    //        vertices.push_back(x);
    //        vertices.push_back(0.5f);
    //        vertices.push_back(z);
    //        vertices.push_back(x + 0.5f);
    //        vertices.push_back(z + 0.5f);
    //        vertices.push_back(0.0f);
    //        vertices.push_back(1.0f);
    //        vertices.push_back(0.0f);
    //
    //        // Side top vertex
    //        vertices.push_back(x);
    //        vertices.push_back(0.5f);
    //        vertices.push_back(z);
    //        vertices.push_back(u);
    //        vertices.push_back(1.0f);
    //        vertices.push_back(x * 2.0f); // Normal points outward
    //        vertices.push_back(0.0f);
    //        vertices.push_back(z * 2.0f);
    //
    //        // Side bottom vertex
    //        vertices.push_back(x);
    //        vertices.push_back(-0.5f);
    //        vertices.push_back(z);
    //        vertices.push_back(u);
    //        vertices.push_back(0.0f);
    //        vertices.push_back(x * 2.0f);
    //        vertices.push_back(0.0f);
    //        vertices.push_back(z * 2.0f);
    //
    //        // Bottom cap vertex
    //        vertices.push_back(x);
    //        vertices.push_back(-0.5f);
    //        vertices.push_back(z);
    //        vertices.push_back(x + 0.5f);
    //        vertices.push_back(z + 0.5f);
    //        vertices.push_back(0.0f);
    //        vertices.push_back(-1.0f);
    //        vertices.push_back(0.0f);
    //    }
    //
    //    // Top cap triangles
    //    for (int i = 0; i < segments; ++i) {
    //        indices.push_back(0);
    //        indices.push_back(2 + i * 4);
    //        indices.push_back(2 + (i + 1) * 4);
    //    }
    //
    //    // Side triangles
    //    for (int i = 0; i < segments; ++i) {
    //        int base = 3 + i * 4;
    //        indices.push_back(base);
    //        indices.push_back(base + 1);
    //        indices.push_back(base + 5);
    //
    //        indices.push_back(base);
    //        indices.push_back(base + 5);
    //        indices.push_back(base + 4);
    //    }
    //
    //    // Bottom cap triangles
    //    for (int i = 0; i < segments; ++i) {
    //        indices.push_back(1);
    //        indices.push_back(5 + (i + 1) * 4);
    //        indices.push_back(5 + i * 4);
    //    }
    //
    //    auto vbo = VertexBuffer::make(vertices);
    //    vbo->setLayout({
    //        {Shader::Type::Float3, "a_pos"},
    //        {Shader::Type::Float2, "a_tex"},
    //        {Shader::Type::Float3, "a_norm"}
    //        });
    //    auto ibo = IndexBuffer::make(indices);
    //    auto vao = VertexArray::make(std::move(vbo), std::move(ibo));
    //    static auto mesh = Mesh::make(std::move(vao));
    //    static auto material = Material::make();
    //    setMaterial(material);
    //    setMesh(mesh);
    //}
    
}