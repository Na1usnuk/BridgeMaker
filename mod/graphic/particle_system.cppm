module;

#include "glm/glm.hpp"

export module bm.gfx.particle_system;

import bm.gfx.obj;
import bm.gfx.buffer.vertex;

import std;

namespace bm::gfx
{
	//export class ParticleSystem : public Object
	//{
	//public:
	//	using Ptr = std::shared_ptr<ParticleSystem>;
	//	using KPtrRef = const Ptr&;
	//	using size_t = std::size_t;
	//	using Particle = struct
	//	{
	//		glm::vec3 position;
	//		//glm::vec3 velocity;
	//		//glm::vec4 color;
	//		float     size;
	//		//float     life; // Remaining life of the particle. if < 0 : dead and unused.
	//	};
	//public:
	//	ParticleSystem();
	//	~ParticleSystem() = default;

	//	void forEach(std::function<void(Particle&)>& function);
	//	void update(float dt);
	//	void render();

	//private:

	//	void init();


	//private:

	//	std::vector<Particle> m_particles;
	//	VertexBufferPtr m_instance_vbo;

	//};
}