module;

#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> 

export module bm.gfx.obj;

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
			glm::vec4 color;
			glm::vec2 tex_coords;
			glm::vec3 normal;
		};

	public:

		const glm::vec3& getPos() const
		{
			return m_translate;
		}

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

	private:

		glm::mat4 m_model = glm::mat4(1.f);

		glm::vec3 m_rotate = glm::vec3(0.f);
		glm::vec3 m_scale = glm::vec3(1.f);
		glm::vec3 m_translate = glm::vec3(0.f);

		Traits<Material>::SPtr m_material;
		Traits<Mesh>::SPtr m_mesh;

	};

	export using ObjectPtr = Traits<Object>::Ptr;

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

}