export module bm.gfx:object;

import std;
import glm;

import :material;
import :mesh;
import :transform;


namespace bm::gfx
{

	export class Object
	{
	public:

		Object(std::string_view name = "Object") : m_name(name) {}
		Object(Handler<Mesh> mesh, Handler<Material> material, std::string_view name = "Object") :
			m_mesh(mesh), m_material(material), m_name(name) {
		}

		Object(const Object&) = delete;
		Object& operator=(const Object&) = delete;
		Object(Object&&) = default;
		Object& operator=(Object&&) = default;
		~Object() = default;

		const std::string& getName() const { return m_name; }
		void setName(std::string_view name) { m_name = name; }

		bool isVisible() const { return m_visible; }
		void setVisibility(bool visible) { m_visible = visible; }

		void setMaterial(Handler<Material> material) { m_material = material; }
		Handler<Material> getMaterial() { return m_material; }

		void setMesh(Handler<Mesh> mesh) { m_mesh = mesh; }
		Handler<Mesh> getMesh() { return m_mesh; }

	public:

		Transform transform;

	private:

		std::string m_name;
		bool m_visible = true;

		Handler<Material> m_material;
		Handler<Mesh> m_mesh;

	};


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

		Type getType() const { return m_type; }

		glm::vec3 getColor() const { return m_color; }
		void setColor(const glm::vec3& color) { m_color = color; }

		float getIntensity() const { return m_intensity; }
		void setIntensity(float intensity) { m_intensity = intensity; }

		glm::vec3 getPosition() const { return m_position; }
		void setPosition(const glm::vec3& position) { m_position = position; }

		glm::vec3 getDirection() const { return m_direction; }
		void setDirection(const glm::vec3& direction) { m_direction = direction; }

	public:

		Transform transform;

	private:

		Type m_type;

		float m_intensity = 1.0f;
		glm::vec3 m_color = glm::vec3(1.0f);
		glm::vec3 m_position = glm::vec3(0.0f);
		glm::vec3 m_direction = glm::vec3(0.0f, -1.0f, 0.0f);

	};

}