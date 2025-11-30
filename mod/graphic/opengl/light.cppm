module;

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <imgui.h>

export module bm.gfx.light;

import std;

import bm.traits;

namespace bm::gfx
{

	export class Light
	{
	public:

		enum class Type
		{
			Directional,
			Point,
			Spot
		};

	public:

		Light(Type type = Type::Directional) : m_type(type) {}
		virtual ~Light() = default;

		virtual void onImGuiRender();

		Type getType() const { return m_type; }

		glm::vec3 getColor() const { return m_color; }
		void setColor(const glm::vec3& color) { m_color = color; }

		float getIntensity() const { return m_intensity; }
		void setIntensity(float intensity) { m_intensity = intensity; }

		glm::vec3 getPosition() const { return m_position; }
		void setPosition(const glm::vec3& position) { m_position = position; }

		glm::vec3 getDirection() const { return m_direction; }
		void setDirection(const glm::vec3& direction) { m_direction = direction; } 

		static Traits<Light>::Ptr make(Type type = Type::Directional)
		{
			return std::make_unique<Light>(type);
		}

	private:

		Type m_type;
		glm::vec3 m_color = glm::vec3(1.0f);
		float m_intensity = 1.0f;
		glm::vec3 m_position = glm::vec3(0.0f);
		glm::vec3 m_direction = glm::vec3(0.0f, -1.0f, 0.0f);

	};

	void Light::onImGuiRender()
	{
		bool changed = false;

		if (ImGui::CollapsingHeader("Light"))
		{
			glm::vec3 pos = m_position;
			if (ImGui::DragFloat3("Position", &pos.x, 0.1f))
			{
				m_position = pos;
				changed = true;
			}

			glm::vec3 color = m_color;
			if (ImGui::ColorEdit3("Color", &color.x))
			{
				m_color = color;
				changed = true;
			}

			float intensity = m_intensity;
			if (ImGui::DragFloat("Intensity", &intensity, 0.05f, 0.0f, 100.0f))
			{
				m_intensity = intensity;
				changed = true;
			}

			if (ImGui::Button("Reset Light"))
			{
				m_position = glm::vec3(0.f);
				m_color = glm::vec3(1.f);
				m_intensity = 1.f;
				changed = true;
			}
		}
	}
}