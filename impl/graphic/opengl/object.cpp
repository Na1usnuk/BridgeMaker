module;

#include <tiny_obj_loader.h>

module bm.gfx:object;
import :object;

import :mesh;
import :material;
import :primitives;

namespace bm::gfx
{
    struct Vertex
    {
        glm::vec3 position;
        glm::vec2 tex_coords;
        glm::vec3 normal;
        glm::vec3 tangent;
    };

    Mesh loadOBJ(const std::filesystem::path& path)
    {
        // Check extension
        ::bm::core::verify(
            path.extension() == ".obj",
            std::format("Object instance can only be constructed from .obj files, got '{}'",
                path.extension().string())
        );

        // Prepare tinyobj structures
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warning;
        std::string error;

        // Optional base directory for .mtl and textures
        const auto base_dir = path.parent_path().string();
        const auto path_str = path.string();

        // Load .obj (use filename overload, force triangulation)
        bool loaded = tinyobj::LoadObj(
            &attrib,
            &shapes,
            &materials,
            &warning,
            &error,
            path_str.c_str(),
            base_dir.empty() ? nullptr : base_dir.c_str(),
            /*triangulate*/ true
        );

        if (!warning.empty())
            ::bm::core::log::warning(
                "Warning while loading OBJ '{}': {}",
                path_str, warning
            );

        ::bm::core::verify(
            loaded,
            std::format("Failed to load OBJ '{}': {}",
                path_str,
                error.empty() ? std::string_view{ "(no error info)" } : std::string_view{ error })
        );

        // Build interleaved vertex buffer: pos(3) + tex(2) + norm(3) = 8 floats per vertex
        std::vector<Vertex> vertices;
        vertices.reserve(shapes.size() * 3 * 3);

        for (const auto& shape : shapes)
        {
            for (const auto& index : shape.mesh.indices)
            {
                Vertex v;

                // Position (3 floats)
                if (index.vertex_index >= 0)
                {
                    const std::size_t vi = static_cast<std::size_t>(index.vertex_index) * 3;
                    if (vi + 2 < attrib.vertices.size())
                    {
                        v.position =
                        {
                            attrib.vertices[vi + 0],
                            attrib.vertices[vi + 1],
                            attrib.vertices[vi + 2]
                        };
                    }
                    else
                    {
                        ::bm::core::log::warning(
                            "Vertex index {} out of range in OBJ '{}'",
                            index.vertex_index, path_str
                        );
                    }
                }

                // Texcoords (2 floats) – optional
                if (index.texcoord_index >= 0)
                {
                    const std::size_t t = static_cast<std::size_t>(index.texcoord_index) * 2;

                    if (t + 1 < attrib.texcoords.size())
                    {
                        v.tex_coords =
                        {
                            attrib.texcoords[t + 0],
                            attrib.texcoords[t + 1]
                        };
                    }
                    else
                    {
                        ::bm::core::log::warning(
                            "Texcoord index {} out of range in OBJ '{}'",
                            index.texcoord_index, path_str
                        );
                    }
                }


                // Normals (3 floats) – optional
                if (index.normal_index >= 0)
                {
                    const std::size_t n = static_cast<std::size_t>(index.normal_index) * 3;
                    if (n + 2 < attrib.normals.size())
                    {
                        v.normal =
                        {
                            attrib.normals[n + 0],
                            attrib.normals[n + 1],
                            attrib.normals[n + 2]
                        };
                    }
                    else
                    {
                        ::bm::core::log::warning(
                            "Normal index {} out of range in OBJ '{}'",
                            index.normal_index, path_str
                        );
                    }
                }

                v.tangent = glm::vec3(0.0f); // will fill later
                vertices.push_back(v);
            }
        }
        // Compute tangents per triangle
        for (std::size_t i = 0; i + 2 < vertices.size(); i += 3)
        {
            Vertex& v0 = vertices[i + 0];
            Vertex& v1 = vertices[i + 1];
            Vertex& v2 = vertices[i + 2];

            glm::vec3 p0 = v0.position;
            glm::vec3 p1 = v1.position;
            glm::vec3 p2 = v2.position;

            glm::vec2 uv0 = v0.tex_coords;
            glm::vec2 uv1 = v1.tex_coords;
            glm::vec2 uv2 = v2.tex_coords;

            glm::vec3 edge1 = p1 - p0;
            glm::vec3 edge2 = p2 - p0;

            glm::vec2 dUV1 = uv1 - uv0;
            glm::vec2 dUV2 = uv2 - uv0;

            float f = (dUV1.x * dUV2.y - dUV2.x * dUV1.y);
            if (fabsf(f) < 1e-6f)
                f = 1.0f;
            else
                f = 1.0f / f;

            glm::vec3 tangent;
            tangent.x = f * (dUV2.y * edge1.x - dUV1.y * edge2.x);
            tangent.y = f * (dUV2.y * edge1.y - dUV1.y * edge2.y);
            tangent.z = f * (dUV2.y * edge1.z - dUV1.y * edge2.z);

            // accumulate tangents, will normalize later
            v0.tangent += tangent;
            v1.tangent += tangent;
            v2.tangent += tangent;
        }

        // Normalize tangents
        for (auto& v : vertices)
        {
            float len2 = glm::dot(v.tangent, v.tangent);
            if (len2 > 1e-8f)
                v.tangent = glm::normalize(v.tangent);
            else
                v.tangent = glm::vec3(1.0f, 0.0f, 0.0f); // fallback
        }


        vertices.shrink_to_fit();

        // Create GPU resources

        VertexArray vao({vertices});
        vao.setLayout
        ({
            {Shader::Type::Float3, "pos"},
            {Shader::Type::Float2, "tex"},
            {Shader::Type::Float3, "uv"},
            {Shader::Type::Float3, "tang"}
        });

        return Mesh(std::move(vao));
    }

    Object::Object(const std::filesystem::path& path, std::string_view name) : 
        m_mesh(loadOBJ(path)),
        m_name(name)
    {
    }
}