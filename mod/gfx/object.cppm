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
		Object(Handle<Mesh> mesh, Handle<Material> material, std::string_view name = "Object") :
			m_mesh(mesh), m_material(material), m_name(name) {
		}

		~Object() = default;
		Object(const Object&) = delete;
		Object& operator=(const Object&) = delete;
		Object(Object&&) noexcept = default;
		Object& operator=(Object&&) noexcept = default;

		const std::string& getName() const { return m_name; }
		void setName(std::string_view name) { m_name = name; }

		bool isVisible() const noexcept { return m_visible; }
		void setVisibility(bool visible) noexcept { m_visible = visible; }

		void setMaterial(Handle<Material> material) noexcept { m_material = material; }
		Handle<Material> getMaterial() const noexcept { return m_material; }

		void setMesh(Handle<Mesh> mesh) noexcept { m_mesh = mesh; }
		Handle<Mesh> getMesh() const noexcept { return m_mesh; }

	public:

		Transform transform;

	private:

		std::string m_name;
		bool m_visible = true;

		Handle<Material> m_material;
		Handle<Mesh> m_mesh;

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