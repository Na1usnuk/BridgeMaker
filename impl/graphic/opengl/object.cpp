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

    struct Vertex
    {
        glm::vec3 position;
        glm::vec2 tex_coords;
        glm::vec3 normal;
        glm::vec3 tangent;
    };

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
                        v.tex_coords =
                        {
                            attrib.texcoords[t + 0],
                            attrib.texcoords[t + 1]
                        };
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
                        v.normal = 
                        {
                            attrib.normals[n + 0],
                            attrib.normals[n + 1],
                            attrib.normals[n + 2]
						};
                    }
                    else
                    {
                        ::bm::log::core::warning(
                            "Normal index {} out of range in OBJ '{}'",
                            index.normal_index, path_str
                        );
                    }
                }

                v.tangent = glm::vec3(0.0f); // will fill later
                vertices.push_back(v);
            }
        }
        // Compute tangents per triangle (assumes triangles because triangulate=true)
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
        auto vbo = VertexBuffer::make(vertices);
        vbo->setLayout(
            {
                { Shader::Type::Float3, "a_pos" },
                { Shader::Type::Float2, "a_tex" },
                { Shader::Type::Float3, "a_norm"},
			    { Shader::Type::Float3, "a_tan"}
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
		// Transform
        if (ImGui::CollapsingHeader("Transform"))
        {

            // Position
            glm::vec3& pos = m_translate;
            if (ImGui::DragFloat3("Position", &pos.x, 0.1f))
                setPosition(pos);

            // Rotation (degrees)
            glm::vec3& rot = m_rotate;
            if (ImGui::DragFloat3("Rotation", &rot.x, 0.1f))
                setRotation(rot);

            // Scale
            glm::vec3& scale = m_scale;
            if (ImGui::DragFloat3("Scale", &scale.x, 0.05f))
                setScale(scale);

            // Reset button
            if (ImGui::Button("Reset Transform"))
            {
                m_translate = glm::vec3(0.f);
                m_rotate = glm::vec3(0.f);
                m_scale = glm::vec3(1.f);
				m_dirty = true;
            }
        }

        ImGui::Separator();

        // Material
        if (ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_DefaultOpen))
        {
            auto mat = m_material;
            if (!mat)
                ImGui::TextDisabled("No material");
            else
            {
				// Color
                ImGui::ColorEdit4("Color", (float*)&mat->getColor());

				// Ambient, Diffuse, Specular
                ImGui::ColorEdit3("Ambient", (float*)&mat->getAmbient());
                ImGui::ColorEdit3("Diffuse", (float*)&mat->getDiffuse());
                ImGui::ColorEdit3("Specular", (float*)&mat->getSpecular());

				// Shininess
                float shininess = mat->getShininess();
                if (ImGui::SliderFloat("Shininess", &shininess, 1.0f, 256.0f))
                    mat->setShininess(shininess);

                // Transparency badge
                if (mat->getColor().a < 1.0f)
                    ImGui::TextColored(ImVec4(0.5f, 0.7f, 1.f, 1.f),
                        "Transparent (alpha = %.2f)", mat->getColor().a);
            }
        }

        ImGui::Separator();

        // Texture
        if (ImGui::CollapsingHeader("Texture", ImGuiTreeNodeFlags_DefaultOpen))
        {
            auto tex = m_material->getTexture();
            if (!tex)
                ImGui::TextDisabled("No texture");
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

		// Normal Map

        ImGui::Separator();

        if (ImGui::CollapsingHeader("Normal Map", ImGuiTreeNodeFlags_DefaultOpen))
        {
            auto mat = m_material;
            auto nmap = mat->getNormalMap();

            // If we reach here → normal map is assigned
            ImGui::SeparatorText("Normal Map Preview");

            if(nmap)
            {
                ImGui::Text("NormalMap ID: %u", nmap->getID());
                ImGui::Image(
                    (ImTextureID)(intptr_t)nmap->getID(),
                    ImVec2(96, 96),
                    ImVec2(0, 1), ImVec2(1, 0)
                );
                ImGui::Text("Size: %dx%d", nmap->getWidth(), nmap->getHeight());
                ImGui::Text("File: %s", nmap->getFilepath().c_str());

                if (ImGui::Button("Reload Normal Map"))
                {
                    auto newTex = Texture::make(nmap->getFilepath());
                    mat->setNormalMap(newTex);
                }

                ImGui::SameLine();
                if (ImGui::Button("Remove"))
                {
                    mat->setNormalMap(nullptr);
                }
            }
            else
            {
                ImGui::TextDisabled("No normal map assigned.");
		}
        }

        ImGui::Separator();

        // Mesh
        if (ImGui::CollapsingHeader("Mesh Info"))
        {
            auto mesh = m_mesh;
            if (!mesh)
                ImGui::TextDisabled("No mesh");
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
                    ImGui::TextDisabled("Mesh has no VAO");
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

		m_normal_matrix = glm::mat3(glm::transpose(glm::inverse(m_model)));

		m_dirty = false;
	}

	void Object::setPositionX(const float x)
	{
		m_translate.x = x;
		m_dirty = true;
	}

	void Object::setPositionY(const float y)
	{
		m_translate.y = y;
        m_dirty = true;
	}

	void Object::setPositionZ(const float z)
	{
        m_translate.z = z;
        m_dirty = true;
	}

	void Object::setRotationX(const float x)
	{
		m_rotate.x = x;
        m_dirty = true;
    }

	void Object::setRotationY(const float y)
	{
		m_rotate.y = y;
        m_dirty = true;
    }

	void Object::setRotationZ(const float z)
	{
		m_rotate.z = z;
        m_dirty = true;
    }

	void Object::setRotation(const glm::vec3& degres)
	{
		m_rotate = degres;
        m_dirty = true;
    }

	void Object::setPosition(const glm::vec3& position)
	{
		m_translate = position;
        m_dirty = true;
    }

	void Object::setScale(const glm::vec3& by)
	{
		m_scale = by;
        m_dirty = true;
    }

	void Object::scale(const glm::vec3& by)
	{
		m_scale *= by;
        m_dirty = true;
    }

	void Object::scaleX(float by)
	{
		m_scale.x *= by;
        m_dirty = true;
    }

	void Object::scaleY(float by)
	{
		m_scale.y *= by;
        m_dirty = true;
    }

	void Object::scaleZ(float by)
	{
		m_scale.z *= by;
        m_dirty = true;
    }

	void Object::move(const glm::vec3& to)
	{
		m_translate += to;
        m_dirty = true;
    }

	void Object::moveX(float to)
	{
		m_translate.x += to;
        m_dirty = true;
    }

	void Object::moveY(float to)
	{
		m_translate.y += to;
        m_dirty = true;
    }

	void Object::moveZ(float to)
	{
		m_translate.z += to;
        m_dirty = true;
    }

	void Object::rotateX(float degres)
	{
		m_rotate.x += degres;
        m_dirty = true;
    }

	void Object::rotateY(float degres)
	{
		m_rotate.y += degres;
        m_dirty = true;
    }

	void Object::rotateZ(float degres)
	{
		m_rotate.z += degres;
        m_dirty = true;
    }

	void Object::rotate(const glm::vec3& degres)
	{
		m_rotate += degres;
        m_dirty = true;
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