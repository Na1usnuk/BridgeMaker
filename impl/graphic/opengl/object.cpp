module;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> 

#include <tiny_obj_loader.h>

module bm.gfx.object;

import std;

import bm.verify;
import bm.log;

import bm.gfx.mesh;
import bm.gfx.material;
import bm.gfx.texture;
import bm.gfx.shader;
import bm.gfx.buffer.vertex;
import bm.gfx.vertexarray;

namespace bm::gfx
{

    Object::Object(const std::filesystem::path& path)
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
            ::bm::log::core::warning(
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
        std::vector<float> vertices;
        vertices.reserve(shapes.size() * 3 /*triangles guess*/ * 8);

        for (const auto& shape : shapes)
        {
            for (const auto& index : shape.mesh.indices)
            {
                glm::vec3 pos{ 0.0f };
                glm::vec2 tex{ 0.0f };
                glm::vec3 norm{ 0.0f };

                // Position (3 floats)
                if (index.vertex_index >= 0)
                {
                    const std::size_t v = static_cast<std::size_t>(index.vertex_index) * 3;
                    if (v + 2 < attrib.vertices.size())
                    {
                        pos.x = attrib.vertices[v + 0];
                        pos.y = attrib.vertices[v + 1];
                        pos.z = attrib.vertices[v + 2];
                    }
                    else
                    {
                        ::bm::log::core::warning(
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
                        tex.x = attrib.texcoords[t + 0];
                        tex.y = attrib.texcoords[t + 1];
                    }
                    else
                    {
                        ::bm::log::core::warning(
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
                        norm.x = attrib.normals[n + 0];
                        norm.y = attrib.normals[n + 1];
                        norm.z = attrib.normals[n + 2];
                    }
                    else
                    {
                        ::bm::log::core::warning(
                            "Normal index {} out of range in OBJ '{}'",
                            index.normal_index, path_str
                        );
                    }
                }

                // Push interleaved data: pos (3) + tex (2) + norm (3)
                vertices.push_back(pos.x);
                vertices.push_back(pos.y);
                vertices.push_back(pos.z);

                vertices.push_back(tex.x);
                vertices.push_back(tex.y);

                vertices.push_back(norm.x);
                vertices.push_back(norm.y);
                vertices.push_back(norm.z);
            }
        }

        // Create GPU resources
        auto vbo = VertexBuffer::make(vertices);
        vbo->setLayout(
            {
                { Shader::Type::Float3, "a_pos" },
                { Shader::Type::Float2, "a_tex" },
                { Shader::Type::Float3, "a_norm" }
            }
        );

        auto vao = VertexArray::make(std::move(vbo));
        auto mesh = Mesh::make(std::move(vao));
        auto material = Material::make();

        setMesh(mesh);
        setMaterial(material);
    }



	void Object::apply()
	{
		m_model = glm::mat4(1.f);
		m_model = glm::translate(m_model, m_translate);
		m_model = glm::rotate(m_model, glm::radians(m_rotate.z), glm::vec3(0.f, 0.f, 1.f));
		m_model = glm::rotate(m_model, glm::radians(m_rotate.y), glm::vec3(0.f, 1.f, 0.f));
		m_model = glm::rotate(m_model, glm::radians(m_rotate.x), glm::vec3(1.f, 0.f, 0.f));
		m_model = glm::scale(m_model, m_scale);
	}

	void Object::setPositionX(const float x)
	{
		m_translate.x = x;
	}

	void Object::setPositionY(const float y)
	{
		m_translate.y = y;
	}

	void Object::setPositionZ(const float z)
	{
		m_translate.z = z;
	}

	void Object::setRotationX(const float x)
	{
		m_rotate.x = x;
	}

	void Object::setRotationY(const float y)
	{
		m_rotate.y = y;
	}

	void Object::setRotationZ(const float z)
	{
		m_rotate.z = z;
	}

	void Object::setRotation(const glm::vec3& degres)
	{
		m_rotate = degres;
	}

	void Object::setPosition(const glm::vec3& position)
	{
		m_translate = position;
	}

	void Object::setScale(const glm::vec3& by)
	{
		m_scale = by;
	}

	void Object::scale(const glm::vec3& by)
	{
		m_scale *= by;
	}

	void Object::scaleX(float by)
	{
		m_scale.x *= by;
	}

	void Object::scaleY(float by)
	{
		m_scale.y *= by;
	}

	void Object::scaleZ(float by)
	{
		m_scale.z *= by;
	}

	void Object::move(const glm::vec3& to)
	{
		m_translate += to;
	}

	void Object::moveX(float to)
	{
		m_translate.x += to;
	}

	void Object::moveY(float to)
	{
		m_translate.y += to;
	}

	void Object::moveZ(float to)
	{
		m_translate.z += to;
	}

	void Object::rotateX(float degres)
	{
		m_rotate.x += degres;
	}

	void Object::rotateY(float degres)
	{
		m_rotate.y += degres;
	}

	void Object::rotateZ(float degres)
	{
		m_rotate.z += degres;
	}

	void Object::rotate(const glm::vec3& degres)
	{
		m_rotate += degres;
	}

	const glm::vec3& Object::getPosition() const
	{
		return m_translate;
	}
	const glm::vec3& Object::getScale() const
	{
		return m_scale;
	}
	const glm::vec3& Object::getRotation() const
	{
		return m_rotate;
	}
}