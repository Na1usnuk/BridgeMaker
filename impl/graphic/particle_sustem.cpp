module;

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "glad/glad.h"

module bm.gfx.particle_system;

import bm.graphic;

namespace bm::gfx
{
	//static const std::string s_vertex_shader_src = 
	//R"(

	//)";
	//static const std::string s_fragment_shader_src =
	//R"(

	//)";
	//struct Vertex
	//{
	//	glm::vec3 position;
	//	glm::vec2 tex_coords;
	//};
	//static const std::array<float, 20> s_quad_vertices = 
	//{
	//	// positions        // texture coords
	//	-0.5f, -0.5f, 0.0f,	0.0f, 0.0f,			
	//	 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
	//	 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
	//	-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
	//};
	//static const std::array<unsigned int, 6> s_quad_indices = 
	//{
	//	0, 1, 2,
	//	2, 3, 0
	//};
	//static const std::array<std::size_t, 2> s_quad_layout = 
	//{
	//	3, 2
	//};

	//static const std::size_t s_max_particles = 10000;
	//static const std::size_t s_max_indices = s_max_particles * 6;
	//static const std::size_t s_max_vertices = s_max_particles * 4;

	//ParticleSystem::ParticleSystem()
	//{
	//}

	//void ParticleSystem::init()
	//{
	//	IndexBufferPtr ibo = IndexBuffer::make(s_quad_indices);
	//	VertexBufferPtr vbo = VertexBuffer::make(s_quad_vertices, VertexBuffer::Draw::DYNAMIC);
	//	for(const auto layout : s_quad_layout)
	//		vbo->pushLayout<float>(layout);

	//	m_instance_vbo = VertexBuffer::make(s_max_vertices * sizeof(Particle), VertexBuffer::Draw::DYNAMIC);

	//	// attribute (2) = position
	//	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)0);
	//	glEnableVertexAttribArray(2);
	//	glVertexAttribDivisor(2, 1);     // <- THIS is instancing

	//	// attribute (3) = size
	//	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)(offsetof(Particle, size)));
	//	glEnableVertexAttribArray(3);
	//	glVertexAttribDivisor(3, 1);     // <- THIS is instancing


	//	VertexArrayPtr vao = VertexArray::make(vbo);

	//	MeshPtr mesh = Mesh::make(vao, ibo);
	//	setMesh(mesh);

	//	MaterialPtr material = Material::make(); // default material
	//	setMaterial(material);

	//}

	//void ParticleSystem::update(float dt)
	//{
	//	//for(auto& particle : m_particles)
	//	//{
	//	//	if(particle.life > 0.f)
	//	//	{
	//	//		particle.position += particle.velocity * dt;
	//	//		particle.life -= dt;
	//	//		if(particle.life < 0.f)
	//	//			particle.life = 0.f;
	//	//	}
	//	//}
	//}

	//void ParticleSystem::forEach(std::function<void(Particle&)>& function)
	//{
	//	for(auto& particle : m_particles)
	//		function(particle);
	//}

	//void ParticleSystem::render()
	//{

	//}
}