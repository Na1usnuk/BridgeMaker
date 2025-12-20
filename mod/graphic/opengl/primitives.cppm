export module bm.gfx:primitives;

import :buffer;
import :array;
import :object;

import std;

namespace bm::gfx
{

    export const VertexBuffer::Layout pos_tex_norm_layout =
    {
        {Shader::Type::Float3, "a_pos"},
        {Shader::Type::Float2, "a_tex"},
        {Shader::Type::Float3, "a_norm"}
    };

    export Object makeTriangle()
    {
        static constexpr std::array<float, 24> vertices =
        {
            // positions       // texture coords    // normals
            0.0f,  0.5f, 0.0f, 0.5f, 1.0f,          0.0f, 0.0f, 1.0f,
            -0.5f,-0.5f, 0.0f, 0.0f, 0.0f,          0.0f, 0.0f, 1.0f,
            0.5f, -0.5f, 0.0f, 1.0f, 0.0f,          0.0f, 0.0f, 1.0f,
        };

        VertexArray vao({ vertices });
        vao.setLayout
        ({
            {Shader::Type::Float3, "a_pos"},
            {Shader::Type::Float2, "a_tex"},
            {Shader::Type::Float3, "a_norm"}
            });

        return Object(Mesh(std::move(vao)));
    }

    export Object makeCube()
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

        VertexArray vao({ vertices }, { indices });
        vao.setLayout
        ({
            {Shader::Type::Float3, "a_pos"},
            {Shader::Type::Float2, "a_tex"},
            {Shader::Type::Float3, "a_norm"}
        });

        Mesh mesh(std::move(vao));

        return Object(std::move(mesh));
    }


    export Object makeSphere(int sectors = 36, int stacks = 18 )
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

        VertexArray vao({ vertices }, { indices });
        vao.setLayout({
            {Shader::Type::Float3, "a_pos"},
            {Shader::Type::Float2, "a_tex"},
            {Shader::Type::Float3, "a_norm"}
        });

        Mesh mesh(std::move(vao));
        return Object(std::move(mesh));
    }

   /* Object makeSquare();

    Object makeCube();

    Object makeGrid(float size_of_line = 100.f, float size_of_square_side = 0.2f);

    Object makeSphere(int sectors = 36, int stacks = 18);

    Object makePyramid();

    Object makeCone(int segments = 36);

    Object makeCylinder(int segments = 36);*/

}