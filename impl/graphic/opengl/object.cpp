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
    };

    Mesh loadOBJ(const std::filesystem::path& path)
    {
        ::bm::core::verify(
            path.extension() == ".obj",
            "Only .obj files are supported"
        );

        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn, err;

        const auto base_dir = path.parent_path().string();
        const auto path_str = path.string();

        bool ok = tinyobj::LoadObj(
            &attrib,
            &shapes,
            &materials,
            &warn,
            &err,
            path_str.c_str(),
            base_dir.empty() ? nullptr : base_dir.c_str(),
            true
        );

        if (!warn.empty())
            ::bm::core::log::warning("OBJ warning: {}", warn);

        ::bm::core::verify(ok, err.empty() ? "OBJ load failed" : err);

        std::vector<Vertex> vertices;
        vertices.reserve(10000);

        for (const auto& shape : shapes)
        {
            for (const auto& idx : shape.mesh.indices)
            {
                Vertex v{};

                if (idx.vertex_index >= 0)
                {
                    size_t i = idx.vertex_index * 3;
                    v.position = {
                        attrib.vertices[i + 0],
                        attrib.vertices[i + 1],
                        attrib.vertices[i + 2]
                    };
                }

                if (idx.normal_index >= 0)
                {
                    size_t i = idx.normal_index * 3;
                    v.normal = {
                        attrib.normals[i + 0],
                        attrib.normals[i + 1],
                        attrib.normals[i + 2]
                    };
                }
                else
                {
                    v.normal = glm::vec3(0, 1, 0);
                }

                if (idx.texcoord_index >= 0)
                {
                    size_t i = idx.texcoord_index * 2;
                    v.tex_coords = {
                        attrib.texcoords[i + 0],
                        attrib.texcoords[i + 1]
                    };
                }

                vertices.push_back(v);
            }
        }

        VertexArray vao({ vertices });
        vao.setLayout({
            { Shader::Type::Float3, "pos" },
            { Shader::Type::Float2, "tex" },
            { Shader::Type::Float3, "normal" }
            });

        return Mesh(std::move(vao));
    }


    Object::Object(const std::filesystem::path& path, std::string_view name) : 
        m_mesh(loadOBJ(path)),
        m_name(name)
    {
    }
}