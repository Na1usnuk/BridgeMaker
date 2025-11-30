export module bm.gfx.primitives;

import bm.gfx.buffer.vertex;
import bm.gfx.buffer.index;
import bm.gfx.vertexarray;
import bm.gfx.shader;
import bm.gfx.mesh;
import bm.gfx.material;
import bm.gfx.object;

import bm.assetmanager;

import std;


namespace bm::gfx
{

export class Triangle : public Object
{
public:

    static constexpr std::array<float, 24> vertices =
    {
		// positions       // texture coords    // normals
        0.0f,  0.5f, 0.0f, 0.5f, 1.0f,          0.0f, 0.0f, 1.0f,
        -0.5f,-0.5f, 0.0f, 0.0f, 0.0f,          0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f,          0.0f, 0.0f, 1.0f,
    };

    static constexpr std::array<unsigned int, 3> indices =
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
        auto mesh = Mesh::make(std::move(vao));
        setMesh(mesh);
        auto material = Material::make();
        setMaterial(material);
    }
};


export class Square : public Object
{
public:

    Square()
	{
        static constexpr std::array<float, 8 * 4> vertices =
        {
            // Position          // TexCoords  // Normal
            -0.5f,  0.0f,  0.5f,  0.0f, 1.0f,   0.0f, 1.0f, 0.0f,  
             0.5f,  0.0f,  0.5f,  1.0f, 1.0f,   0.0f, 1.0f, 0.0f,
             0.5f,  0.0f, -0.5f,  1.0f, 0.0f,   0.0f, 1.0f, 0.0f,
            -0.5f,  0.0f, -0.5f,  0.0f, 0.0f,   0.0f, 1.0f, 0.0f
        };

        static constexpr std::array<unsigned int, 6> indices =
        {
            0, 1, 2,
            2, 3, 0
        };

        auto vbo = VertexBuffer::make(vertices);
        vbo->setLayout
        ({
            {Shader::Type::Float3, "a_pos"},
            {Shader::Type::Float2, "a_tex"},
            {Shader::Type::Float3, "a_norm"}
        });
        auto ibo = IndexBuffer::make(indices);
        auto vao = VertexArray::make(std::move(vbo), std::move(ibo));

        auto mesh = Mesh::make(std::move(vao));
        auto material = Material::make();

        setMaterial(material);
        setMesh(mesh);
	}

private:
};


export class Cube : public Object
{

public:

    Cube()
    {
        static constexpr std::array<float, 192> vertices =
        {
            // Position          // TexCoords  // Normal
            // Back face (Z-)
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,   0.0f,  0.0f, -1.0f,
             0.5f, -0.5f, -0.5f,  1.0f, 0.0f,   0.0f,  0.0f, -1.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,   0.0f,  0.0f, -1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,   0.0f,  0.0f, -1.0f,

            // Front face (Z+)
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,   0.0f,  0.0f,  1.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,   0.0f,  0.0f,  1.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 1.0f,   0.0f,  0.0f,  1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,   0.0f,  0.0f,  1.0f,

            // Left face (X-)
            -0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  -1.0f,  0.0f,  0.0f,
            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  -1.0f,  0.0f,  0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,  -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  -1.0f,  0.0f,  0.0f,

            // Right face (X+)
             0.5f, -0.5f, -0.5f,  1.0f, 0.0f,   1.0f,  0.0f,  0.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,   1.0f,  0.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  0.0f, 1.0f,   1.0f,  0.0f,  0.0f,
             0.5f, -0.5f,  0.5f,  0.0f, 0.0f,   1.0f,  0.0f,  0.0f,

             // Bottom face (Y-)
             -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,   0.0f, -1.0f,  0.0f,
              0.5f, -0.5f, -0.5f,  1.0f, 1.0f,   0.0f, -1.0f,  0.0f,
              0.5f, -0.5f,  0.5f,  1.0f, 0.0f,   0.0f, -1.0f,  0.0f,
             -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,   0.0f, -1.0f,  0.0f,

             // Top face (Y+)
             -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,   0.0f,  1.0f,  0.0f,
              0.5f,  0.5f, -0.5f,  1.0f, 1.0f,   0.0f,  1.0f,  0.0f,
              0.5f,  0.5f,  0.5f,  1.0f, 0.0f,   0.0f,  1.0f,  0.0f,
             -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,   0.0f,  1.0f,  0.0f,
        };

        static constexpr std::array<unsigned int, 36> indices = 
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

        auto vbo = VertexBuffer::make(vertices);
        vbo->setLayout
        ({
            {Shader::Type::Float3, "a_pos"},
            {Shader::Type::Float2, "a_tex"},
            {Shader::Type::Float3, "a_norm"}
        });

        auto ibo = IndexBuffer::make(indices);

        auto vao = VertexArray::make(std::move(vbo), std::move(ibo));

        auto mesh = Mesh::make(std::move(vao));

        auto material = Material::make();

        setMaterial(material);
        setMesh(mesh);
    }
};


export class Grid : public Object
{
public:

    Grid(float size_of_line = 100.f, float size_of_square_side = 0.2f)
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


// Sphere
export class Sphere : public Object
{
public:
    Sphere(int sectors = 36, int stacks = 18)
    {
        std::vector<float> vertices;
        std::vector<unsigned int> indices;

        float sectorStep = 2 * 3.14159f / sectors;
        float stackStep = 3.14159f / stacks;

        // Generate vertices
        for (int i = 0; i <= stacks; ++i) {
            float stackAngle = 3.14159f / 2 - i * stackStep;
            float xy = 0.5f * std::cosf(stackAngle);
            float z = 0.5f * std::sinf(stackAngle);

            for (int j = 0; j <= sectors; ++j) {
                float sectorAngle = j * sectorStep;

                float x = xy * std::cosf(sectorAngle);
                float y = xy * std::sinf(sectorAngle);

                // Position
                vertices.push_back(x);
                vertices.push_back(y);
                vertices.push_back(z);

                // Texture coordinates
                vertices.push_back((float)j / sectors);
                vertices.push_back((float)i / stacks);

                // Normal (for sphere, normalized position IS the normal)
                float length = std::sqrtf(x * x + y * y + z * z);
                vertices.push_back(x / length);
                vertices.push_back(y / length);
                vertices.push_back(z / length);
            }
        }

        // Generate indices
        for (int i = 0; i < stacks; ++i) {
            int k1 = i * (sectors + 1);
            int k2 = k1 + sectors + 1;

            for (int j = 0; j < sectors; ++j, ++k1, ++k2) {
                if (i != 0) {
                    indices.push_back(k1);
                    indices.push_back(k2);
                    indices.push_back(k1 + 1);
                }

                if (i != (stacks - 1)) {
                    indices.push_back(k1 + 1);
                    indices.push_back(k2);
                    indices.push_back(k2 + 1);
                }
            }
        }

        auto vbo = VertexBuffer::make(vertices);
        vbo->setLayout({
            {Shader::Type::Float3, "a_pos"},
            {Shader::Type::Float2, "a_tex"},
            {Shader::Type::Float3, "a_norm"}
            });
        auto ibo = IndexBuffer::make(indices);
        auto vao = VertexArray::make(std::move(vbo), std::move(ibo));
        auto mesh = Mesh::make(std::move(vao));
        auto material = Material::make();
        setMaterial(material);
        setMesh(mesh);
    }
};

// Pyramid (square base)
export class Pyramid : public Object
{
public:
    Pyramid()
    {
        static std::array<float, 8 * 16> vertices = {
            // Base (bottom face, normal pointing down)
            -0.5f, 0.0f, -0.5f,  0.0f, 0.0f,  0.0f, -1.0f, 0.0f,
             0.5f, 0.0f, -0.5f,  1.0f, 0.0f,  0.0f, -1.0f, 0.0f,
             0.5f, 0.0f,  0.5f,  1.0f, 1.0f,  0.0f, -1.0f, 0.0f,
            -0.5f, 0.0f,  0.5f,  0.0f, 1.0f,  0.0f, -1.0f, 0.0f,

            // Front face
            -0.5f, 0.0f,  0.5f,  0.0f, 0.0f,  0.0f, 0.447f, 0.894f,
             0.5f, 0.0f,  0.5f,  1.0f, 0.0f,  0.0f, 0.447f, 0.894f,
             0.0f, 1.0f,  0.0f,  0.5f, 1.0f,  0.0f, 0.447f, 0.894f,

             // Right face
              0.5f, 0.0f,  0.5f,  0.0f, 0.0f,  0.894f, 0.447f, 0.0f,
              0.5f, 0.0f, -0.5f,  1.0f, 0.0f,  0.894f, 0.447f, 0.0f,
              0.0f, 1.0f,  0.0f,  0.5f, 1.0f,  0.894f, 0.447f, 0.0f,

              // Back face
               0.5f, 0.0f, -0.5f,  0.0f, 0.0f,  0.0f, 0.447f, -0.894f,
              -0.5f, 0.0f, -0.5f,  1.0f, 0.0f,  0.0f, 0.447f, -0.894f,
               0.0f, 1.0f,  0.0f,  0.5f, 1.0f,  0.0f, 0.447f, -0.894f,

               // Left face
               -0.5f, 0.0f, -0.5f,  0.0f, 0.0f,  -0.894f, 0.447f, 0.0f,
               -0.5f, 0.0f,  0.5f,  1.0f, 0.0f,  -0.894f, 0.447f, 0.0f,
                0.0f, 1.0f,  0.0f,  0.5f, 1.0f,  -0.894f, 0.447f, 0.0f,
        };

        static std::array<unsigned int, 18> indices = {
            // Base
            0, 1, 2,
            2, 3, 0,
            // Faces
            4, 5, 6,
            7, 8, 9,
            10, 11, 12,
            13, 14, 15
        };

        auto vbo = VertexBuffer::make(vertices);
        vbo->setLayout({
            {Shader::Type::Float3, "a_pos"},
            {Shader::Type::Float2, "a_tex"},
            {Shader::Type::Float3, "a_norm"}
            });
        auto ibo = IndexBuffer::make(indices);
        auto vao = VertexArray::make(std::move(vbo), std::move(ibo));
        static auto mesh = Mesh::make(std::move(vao));
        static auto material = Material::make();
        setMaterial(material);
        setMesh(mesh);
    }
};

// Cone
// Cone
export class Cone : public Object
{
public:
    Cone(int segments = 36)
    {
        std::vector<float> vertices;
        std::vector<unsigned int> indices;

        float angleStep = 2 * 3.14159f / segments;

        // Tip vertex
        vertices.insert(vertices.end(), { 0.0f, 1.0f, 0.0f, 0.5f, 1.0f, 0.0f, 1.0f, 0.0f });

        // Base center
        vertices.insert(vertices.end(), { 0.0f, 0.0f, 0.0f, 0.5f, 0.0f, 0.0f, -1.0f, 0.0f });

        // Side vertices and base vertices
        for (int i = 0; i <= segments; ++i) {
            float angle = i * angleStep;
            float x = 0.5f * cosf(angle);
            float z = 0.5f * sinf(angle);

            // Calculate side normal (manually)
            // For a cone, the normal at the side points outward and slightly up
            // Simple approach: normal = normalize(vec3(x, 0.5, z))
            float nx = x;
            float ny = 0.5f;  // Cone slope
            float nz = z;
            float length = sqrtf(nx * nx + ny * ny + nz * nz);
            nx /= length;
            ny /= length;
            nz /= length;

            // Side vertex
            vertices.push_back(x);
            vertices.push_back(0.0f);
            vertices.push_back(z);
            vertices.push_back((float)i / segments);
            vertices.push_back(0.0f);
            vertices.push_back(nx);
            vertices.push_back(ny);
            vertices.push_back(nz);

            // Base vertex
            vertices.push_back(x);
            vertices.push_back(0.0f);
            vertices.push_back(z);
            vertices.push_back(x + 0.5f);
            vertices.push_back(z + 0.5f);
            vertices.push_back(0.0f);
            vertices.push_back(-1.0f);
            vertices.push_back(0.0f);
        }

        // Side triangles
        for (int i = 0; i < segments; ++i) {
            indices.push_back(0); // Tip
            indices.push_back(2 + i * 2);
            indices.push_back(2 + (i + 1) * 2);
        }

        // Base triangles
        for (int i = 0; i < segments; ++i) {
            indices.push_back(1); // Base center
            indices.push_back(3 + (i + 1) * 2);
            indices.push_back(3 + i * 2);
        }

        auto vbo = VertexBuffer::make(vertices);
        vbo->setLayout({
            {Shader::Type::Float3, "a_pos"},
            {Shader::Type::Float2, "a_tex"},
            {Shader::Type::Float3, "a_norm"}
            });
        auto ibo = IndexBuffer::make(indices);
        auto vao = VertexArray::make(std::move(vbo), std::move(ibo));
        static auto mesh = Mesh::make(std::move(vao));
        static auto material = Material::make();
        setMaterial(material);
        setMesh(mesh);
    }
};

// Cylinder
export class Cylinder : public Object
{
public:
    Cylinder(int segments = 36)
    {
        std::vector<float> vertices;
        std::vector<unsigned int> indices;

        float angleStep = 2 * 3.14159f / segments;

        // Top center
        vertices.insert(vertices.end(), { 0.0f, 0.5f, 0.0f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f });

        // Bottom center
        vertices.insert(vertices.end(), { 0.0f, -0.5f, 0.0f, 0.5f, 0.5f, 0.0f, -1.0f, 0.0f });

        // Generate side vertices (duplicated for top, side, and bottom)
        for (int i = 0; i <= segments; ++i) {
            float angle = i * angleStep;
            float x = 0.5f * cosf(angle);
            float z = 0.5f * sinf(angle);
            float u = (float)i / segments;

            // Top cap vertex
            vertices.push_back(x);
            vertices.push_back(0.5f);
            vertices.push_back(z);
            vertices.push_back(x + 0.5f);
            vertices.push_back(z + 0.5f);
            vertices.push_back(0.0f);
            vertices.push_back(1.0f);
            vertices.push_back(0.0f);

            // Side top vertex
            vertices.push_back(x);
            vertices.push_back(0.5f);
            vertices.push_back(z);
            vertices.push_back(u);
            vertices.push_back(1.0f);
            vertices.push_back(x * 2.0f); // Normal points outward
            vertices.push_back(0.0f);
            vertices.push_back(z * 2.0f);

            // Side bottom vertex
            vertices.push_back(x);
            vertices.push_back(-0.5f);
            vertices.push_back(z);
            vertices.push_back(u);
            vertices.push_back(0.0f);
            vertices.push_back(x * 2.0f);
            vertices.push_back(0.0f);
            vertices.push_back(z * 2.0f);

            // Bottom cap vertex
            vertices.push_back(x);
            vertices.push_back(-0.5f);
            vertices.push_back(z);
            vertices.push_back(x + 0.5f);
            vertices.push_back(z + 0.5f);
            vertices.push_back(0.0f);
            vertices.push_back(-1.0f);
            vertices.push_back(0.0f);
        }

        // Top cap triangles
        for (int i = 0; i < segments; ++i) {
            indices.push_back(0);
            indices.push_back(2 + i * 4);
            indices.push_back(2 + (i + 1) * 4);
        }

        // Side triangles
        for (int i = 0; i < segments; ++i) {
            int base = 3 + i * 4;
            indices.push_back(base);
            indices.push_back(base + 1);
            indices.push_back(base + 5);

            indices.push_back(base);
            indices.push_back(base + 5);
            indices.push_back(base + 4);
        }

        // Bottom cap triangles
        for (int i = 0; i < segments; ++i) {
            indices.push_back(1);
            indices.push_back(5 + (i + 1) * 4);
            indices.push_back(5 + i * 4);
        }

        auto vbo = VertexBuffer::make(vertices);
        vbo->setLayout({
            {Shader::Type::Float3, "a_pos"},
            {Shader::Type::Float2, "a_tex"},
            {Shader::Type::Float3, "a_norm"}
            });
        auto ibo = IndexBuffer::make(indices);
        auto vao = VertexArray::make(std::move(vbo), std::move(ibo));
        static auto mesh = Mesh::make(std::move(vao));
        static auto material = Material::make();
        setMaterial(material);
        setMesh(mesh);
    }
};




}