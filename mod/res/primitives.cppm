export module bm.res:primitives;

import bm.gfx;

import std;
import glm;

namespace bm::res
{

    export gfx::Mesh createTriangleMesh()
    {
        gfx::Mesh mesh(gfx::Mesh::Topology::Triangles);

        gfx::VertexLayout layout;
        layout.attributes.emplace_back(gfx::VertexLayout::Type::Float3); // Position: vec3
        auto layout_handle = gfx::ResourceManager::load(std::move(layout));

        struct Vertex 
        {
            float x, y, z;
        };

        std::array<Vertex, 3> vertices = 
        {
            Vertex{ -0.5f, -0.5f, 0.0f },  // Bottom-left
            Vertex{  0.5f, -0.5f, 0.0f },  // Bottom-right
            Vertex{  0.0f,  0.5f, 0.0f }   // Top
        };

        // Set vertex data
        mesh.setVertexData(vertices, layout_handle, gfx::Usage::Const);
        mesh.setVertexLayout(layout_handle);

        return mesh;
    }

}