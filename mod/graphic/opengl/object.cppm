export module bm.gfx:object;

import std;
import glm;

import :material;
import :mesh;


namespace bm::gfx
{
	Mesh loadOBJ(const std::filesystem::path& path);

	export class Object
	{
	public:

		Object(Mesh&& mesh, Material&& material, std::string_view name = "Object") :
			m_mesh(std::move(mesh)), m_material(std::move(material)), m_name(name) {
		}
		Object(Mesh&& mesh, std::string_view name = "Object") :
			m_mesh(std::move(mesh)), m_name(name) {
		}

		Object(const std::filesystem::path& path, std::string_view name = "Object"); // Import .obj file

		Object(const Object&) = delete;
		Object& operator=(const Object&) = delete;

		Object(Object&&) = default;
		Object& operator=(Object&&) = default;

		~Object() = default;

		const std::string& getName() const { return m_name; }
		void setName(std::string_view name) { m_name = name; }

		void setMaterial(Material&& material) { m_material = std::move(material); }
		Material& getMaterial() { return m_material; }
		const Material& getMaterial() const { return m_material; }

		void setMesh(Mesh&& mesh) { m_mesh = std::move(mesh); }
		Mesh& getMesh() { return m_mesh; }
		const Mesh& getMesh() const { return m_mesh; }

		template<typename... Args>
		static Traits<Object>::Ptr make(Args&&... args) { return std::make_unique<Object>(std::forward<Args>(args)...); }

	public:

		Transform transform;

	private:

		Material m_material;
		Mesh m_mesh;

		std::string m_name;

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

	private:

		Type m_type;
		glm::vec3 m_color = glm::vec3(1.0f);
		float m_intensity = 1.0f;
		glm::vec3 m_position = glm::vec3(0.0f);
		glm::vec3 m_direction = glm::vec3(0.0f, -1.0f, 0.0f);

	};

}