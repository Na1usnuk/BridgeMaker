export module bm.gfx:transform;

import glm;

namespace bm::gfx
{
	export class Transform
	{
	public:

		const glm::mat4& getModel();
		const glm::mat3& getNormalMatrix();

		const glm::vec3& getPosition() const;
		const glm::vec3& getScale() const;
		const glm::vec3& getRotation() const;

		void setRotation(const glm::vec3& degres);
		void setPosition(const glm::vec3& to);
		void setScale(const glm::vec3& by);

		void rotate(const glm::vec3& degres);
		void scale(const glm::vec3& by);
		void move(const glm::vec3& to);

	private:

		void apply();

	private:

		glm::mat4 m_model = glm::mat4(1.f);
		glm::mat3 m_normal_matrix = glm::mat3(1.f);

		glm::vec3 m_rotation = glm::vec3(0.f);
		glm::vec3 m_scale = glm::vec3(1.f);
		glm::vec3 m_translation = glm::vec3(0.f);

		bool m_dirty = false;
	};
}