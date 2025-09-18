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

		void setColor(const glm::vec4& color) { m_color = color; }
		const glm::vec4& getColor() const { return m_color; }

		void apply();

		const glm::mat4& getModel() const { return m_model; }

		template<DerivedFromObject T, typename... Args>
		static std::shared_ptr<T> make(Args&&... args) { return std::make_shared<T>(std::forward<Args>(args)...); }

	private:

		glm::mat4 m_model = glm::mat4(1.f);

		glm::vec3 m_rotate = glm::vec3(0.f);
		glm::vec3 m_scale = glm::vec3(1.f);
		glm::vec3 m_translate = glm::vec3(0.f);

		glm::vec4 m_color = glm::vec4(1.f);

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