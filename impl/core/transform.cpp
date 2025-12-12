module bm.core:transform;

import :transform;

import glm;

namespace bm
{
	void Transform::apply()
	{
		m_model = glm::mat4(1.f);
		m_model = glm::translate(m_model, m_translation);
		m_model = glm::rotate(m_model, glm::radians(m_rotation.z), glm::vec3(0.f, 0.f, 1.f));
		m_model = glm::rotate(m_model, glm::radians(m_rotation.y), glm::vec3(0.f, 1.f, 0.f));
		m_model = glm::rotate(m_model, glm::radians(m_rotation.x), glm::vec3(1.f, 0.f, 0.f));
		m_model = glm::scale(m_model, m_scale);

		m_normal_matrix = glm::mat3(glm::transpose(glm::inverse(m_model)));

		m_dirty = false;
	}

	const glm::mat4& Transform::getModel()
	{
		if (m_dirty)
			apply();
		return m_model;
	}

	const glm::mat3& Transform::getNormalMatrix()
	{
		if (m_dirty)
			apply();
		return m_normal_matrix;
	}

	const glm::vec3& Transform::getPosition() const
	{
		return m_translation;
	}

	const glm::vec3& Transform::getScale() const
	{
		return m_scale;
	}

	const glm::vec3& Transform::getRotation() const
	{
		return m_rotation;
	}

	void Transform::setRotation(const glm::vec3& degres)
	{
		m_rotation = degres;
		m_dirty = true;
	}

	void Transform::setPosition(const glm::vec3& position)
	{
		m_translation = position;
		m_dirty = true;
	}

	void Transform::setScale(const glm::vec3& scale)
	{
		m_scale = scale;
		m_dirty = true;
	}

	void Transform::rotate(const glm::vec3& degres)
	{
		m_rotation += degres;
		m_dirty = true;
	}

	void Transform::scale(const glm::vec3& scale)
	{
		m_scale += scale;
		m_dirty = true;
	}

	void Transform::move(const glm::vec3& translation)
	{
		m_translation += translation;
		m_dirty = true;
	}
}