module;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> 

export module bm.gfx.object;

import std;

import bm.gfx.mesh;
import bm.gfx.material;
import bm.gfx.texture;

import bm.traits;


namespace bm::gfx
{

	export class Object
	{
	public:

		struct Vertex
		{
			glm::vec3 position;
			glm::vec2 tex_coords;
			glm::vec3 normal;
		};

	public:


		Object(Traits<Mesh>::SPtr mesh = nullptr, Traits<Material>::SPtr material = nullptr) :
			m_mesh(mesh), m_material(material) { }

		Object(const std::filesystem::path& path);


		const glm::vec3& getPosition() const;
		const glm::vec3& getScale() const;
		const glm::vec3& getRotation() const;

		void rotateX(float degres);
		void rotateY(float degres);
		void rotateZ(float degres);
		void rotate(const glm::vec3& degres);

		void scaleX(float by);
		void scaleY(float by);
		void scaleZ(float by);
		void scale(const glm::vec3& by);

		void moveX(float to);
		void moveY(float to);
		void moveZ(float to);
		void move(const glm::vec3& to);

		//not acumulative versions
		void setRotation(const glm::vec3& degres);
		void setPosition(const glm::vec3& to);
		void setScale(const glm::vec3& by);

		void setPositionX(const float x);
		void setPositionY(const float y);
		void setPositionZ(const float z);

		void setRotationX(const float x);
		void setRotationY(const float y);
		void setRotationZ(const float z);

		void setMaterial(Traits<Material>::KSPtrRef material) { m_material = material; }
		Traits<Material>::SPtr getMaterial() { return m_material; }
		void setMesh(Traits<Mesh>::KSPtrRef mesh) { m_mesh = mesh; }
		Traits<Mesh>::SPtr getMesh() { return m_mesh; }

		void setColor(const glm::vec3& color) { m_material->setColor(glm::vec4(color[0], color[1], color[2], 1.f)); }
		void setTexture(Traits<Texture>::KSPtrRef texture) { m_material->setTexture(texture); }

		void apply();

		const glm::mat4& getModel() const { return m_model; }

		template<typename T, typename... Args>
		requires std::is_base_of_v<Object, T>
		static Traits<T>::Ptr make(Args&&... args) { return std::make_unique<T>(std::forward<Args>(args)...); }

		template<typename... Args>
		static Traits<Object>::Ptr make(Args&&... args) { return std::make_unique<Object>(std::forward<Args>(args)...); }

	private:

		glm::mat4 m_model = glm::mat4(1.f);

		glm::vec3 m_rotate = glm::vec3(0.f);
		glm::vec3 m_scale = glm::vec3(1.f);
		glm::vec3 m_translate = glm::vec3(0.f);

		Traits<Material>::SPtr m_material;
		Traits<Mesh>::SPtr m_mesh;

	};

}