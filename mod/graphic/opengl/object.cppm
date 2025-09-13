module;

#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> 

export module bm.gfx.obj;

import std;

import bm.gfx.mesh;


namespace bm::gfx
{
	export class Object;

	template<typename T>
	concept DerivedFromObject = std::is_base_of_v<Object, T>;

	export class Object : public Mesh
	{
	public:

		using Ptr = std::shared_ptr<Object>;
		using KPtrRef = const Ptr&;

	public:

		void rotateX(float degres);
		void rotateY(float degres);
		void rotateZ(float degres);
		void rotate(const glm::vec3& degres);
		void scale(float amount);
		void move(const glm::vec3& to);

		void apply();

		const glm::mat4& getModel() const { return m_model; }

		template<DerivedFromObject T>
		static std::shared_ptr<T> make() { return std::make_shared<T>(); }
		static Ptr make() { return std::make_shared<Object>(); }

	private:

		glm::mat4 m_model = glm::mat4(1.f);

		glm::vec3 m_rotate = glm::vec3(0.f);
		glm::vec3 m_scale = glm::vec3(1.f);
		glm::vec3 m_translate = glm::vec3(0.f);

	};

	export using ObjectPtr = Object::Ptr;

	void Object::apply()
	{
		m_model = glm::mat4(1.f);
		m_model = glm::translate(m_model, m_translate);
		m_model = glm::rotate(m_model, glm::radians(m_rotate.z), glm::vec3(0.f, 0.f, 1.f));
		m_model = glm::rotate(m_model, glm::radians(m_rotate.y), glm::vec3(0.f, 1.f, 0.f));
		m_model = glm::rotate(m_model, glm::radians(m_rotate.x), glm::vec3(1.f, 0.f, 0.f));
		m_model = glm::scale(m_model, m_scale);
	}

	void Object::scale(float x)
	{
		m_scale *= glm::vec3(x);
	}

	void Object::move(const glm::vec3& to)
	{
		m_translate += to;
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