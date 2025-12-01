module;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> 

#include <tiny_obj_loader.h>

#include <imgui.h>

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

    Object::Object(const std::filesystem::path& path, std::string_view name) : m_name(name)
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
                        tex.y = 1.0f - attrib.texcoords[t + 1];
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


    void Object::onImGuiRender()
    {
        if (ImGui::CollapsingHeader("Transform"))
        {
            bool changed = false;

            // POSITION
            glm::vec3 pos = m_translate;
            if (ImGui::DragFloat3("Position", &pos.x, 0.1f))
            {
                m_translate = pos;
                changed = true;
            }

            // ROTATION (degrees)
            glm::vec3 rot = m_rotate;
            if (ImGui::DragFloat3("Rotation", &rot.x, 0.1f))
            {
                m_rotate = rot;
                changed = true;
            }

            // SCALE
            glm::vec3 scale = m_scale;
            if (ImGui::DragFloat3("Scale", &scale.x, 0.05f))
            {
                m_scale = scale;
                changed = true;
            }

            // RESET BUTTON
            if (ImGui::Button("Reset Transform"))
            {
                m_translate = glm::vec3(0.f);
                m_rotate = glm::vec3(0.f);
                m_scale = glm::vec3(1.f);
                changed = true;
            }

            // APPLY TO MODEL MATRIX
            if (changed)
                apply();
        }

        ImGui::Separator();

        // =============================
   // MATERIAL
   // =============================
        if (ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_DefaultOpen))
        {
            auto mat = m_material;
            if (!mat)
            {
                ImGui::TextDisabled("No material");
            }
            else
            {
                ImGui::ColorEdit4("Color", (float*)&mat->getColor());

                ImGui::ColorEdit3("Ambient", (float*)&mat->getAmbient());
                ImGui::ColorEdit3("Diffuse", (float*)&mat->getDiffuse());
                ImGui::ColorEdit3("Specular", (float*)&mat->getSpecular());

                float shininess = mat->getShininess();
                if (ImGui::SliderFloat("Shininess", &shininess, 1.0f, 256.0f))
                    mat->setShininess(shininess);

                // Transparency badge
                if (mat->getColor().a < 1.0f)
                {
                    ImGui::TextColored(ImVec4(0.5f, 0.7f, 1.f, 1.f),
                        "Transparent (alpha = %.2f)", mat->getColor().a);
                }
            }
        }

        ImGui::Separator();

        // =============================
        // TEXTURE
        // =============================
        if (ImGui::CollapsingHeader("Texture", ImGuiTreeNodeFlags_DefaultOpen))
        {
            auto tex = m_material->getTexture();

            if (!tex)
            {
                ImGui::TextDisabled("No texture");
            }
            else
            {
                ImGui::Text("Texture ID: %u", tex->getID());

                // Preview
                ImGui::Image(
                    (ImTextureID)(intptr_t)tex->getID(),
                    ImVec2(96, 96),
                    ImVec2(0, 1), ImVec2(1, 0)
                );

                ImGui::Text("Size: %dx%d", tex->getWidth(), tex->getHeight());
                ImGui::Text("File: %s", tex->getFilepath().c_str());

                ImGui::SeparatorText("Sampling");

                // Wrappering
                static const char* wrap_names[] = { "Repeat", "Mirrored Repeat" };
                static Texture::Wrappering wraps[] = {
                    Texture::Wrappering::REPEAT,
                    Texture::Wrappering::MiRRORED_REPEAT
                };

                static int wrap_index = 0;
                if (ImGui::Combo("Wrap Mode", &wrap_index, wrap_names, 2))
                    tex->setWrappering(true, true, wraps[wrap_index]);

                // Filtering
                static const char* filter_names[] = { "Nearest", "Linear" };
                static Texture::Filtering filters[] = {
                    Texture::Filtering::NEAREST,
                    Texture::Filtering::LINEAR
                };

                static int filter_index = 0;
                if (ImGui::Combo("Filtering", &filter_index, filter_names, 2))
                    tex->setFiltering(true, true, filters[filter_index]);

                if (ImGui::Button("Reload Texture"))
                {
                    auto new_tex = Texture::make(tex->getFilepath());
                    m_material->setTexture(new_tex);
                }
            }
        }

        ImGui::Separator();

        // =============================
        // MESH INFO
        // =============================
        if (ImGui::CollapsingHeader("Mesh Info"))
        {
            auto mesh = m_mesh;
            if (!mesh)
            {
                ImGui::TextDisabled("No mesh");
            }
            else
            {
                auto& vao = mesh->getVertexArray();
                if (vao)
                {
                    ImGui::Text("Vertices: %d", vao->getVerticesCount());
                    if (vao->getIndexBuffer())
                        ImGui::Text("Indices: %d", vao->getIndexBuffer()->count());
                    else
                        ImGui::Text("Index buffer: none");

                    ImGui::Text("Draw mode: %s",
                        mesh->getDrawAs() == Mesh::DrawAs::Triangles ? "Triangles" : "Lines");

                    if (ImGui::Button("Draw as Triangles"))
                        mesh->setDrawAs(Mesh::DrawAs::Triangles);
                    if (ImGui::Button("Draw as Lines"))
                        mesh->setDrawAs(Mesh::DrawAs::Lines);
                    if (ImGui::Button("Draw as Points"))
                        mesh->setDrawAs(Mesh::DrawAs::Points);

                }
                else
                {
                    ImGui::TextDisabled("Mesh has no VAO");
                }
            }
        }
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