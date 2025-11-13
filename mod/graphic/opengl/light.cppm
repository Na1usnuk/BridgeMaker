module;

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

export module bm.gfx.light;

import std;

namespace bm::gfx
{
	export enum class LightType
	{
		Directional,
		Point,
		Spot
	};

	export class Light
	{
	public:
		using Ptr = std::shared_ptr<Light>;
		using KPtrRef = const Ptr&;
	public:
		Light(LightType type) : m_type(type) {}
		virtual ~Light() = default;
		LightType getType() const { return m_type; }
		glm::vec3 getColor() const { return m_color; }
		void setColor(const glm::vec3& color) { m_color = color; }
		float getIntensity() const { return m_intensity; }
		void setIntensity(float intensity) { m_intensity = intensity; }
		glm::vec3 getPosition() const { return m_position; }
		void setPosition(const glm::vec3& position) { m_position = position; }
		glm::vec3 getDirection() const { return m_direction; }
		void setDirection(const glm::vec3& direction) { m_direction = direction; } 
	private:
		LightType m_type;
		glm::vec3 m_color = glm::vec3(1.0f);
		float m_intensity = 1.0f;
		glm::vec3 m_position = glm::vec3(0.0f);
		glm::vec3 m_direction = glm::vec3(0.0f, -1.0f, 0.0f);
	};
	export using LightPtr = Light::Ptr;
}