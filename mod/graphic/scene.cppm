export module bm.gfx:scene;

import std;

import :mesh;
import :material;

import bm.core;

namespace bm::gfx
{

	export class Object
	{
	public:

		Object(Traits<Mesh>::SPtr mesh = nullptr, Traits<Material>::SPtr material = nullptr, std::string_view name = "Object") :
			m_mesh(mesh), m_material(material), m_name(name) {}

		Object(const std::filesystem::path& path, std::string_view name = "Object");

		const std::string& getName() const { return m_name; }
		void setName(std::string_view name) { m_name = name; }

		void setMaterial(Traits<Material>::KSPtrRef material) { m_material = material; }
		Traits<Material>::SPtr getMaterial() { return m_material; }
		void setMesh(Traits<Mesh>::KSPtrRef mesh) { m_mesh = mesh; }
		Traits<Mesh>::SPtr getMesh() { return m_mesh; }

		const Transform& getTransform() const { return m_transform; }
		Transform& getTransform() { return m_transform; }
		void setTransform(const Transform& transform) { m_transform = transform; }

		template<typename T, typename... Args>
			requires std::is_base_of_v<Object, T>
		static Traits<T>::Ptr make(Args&&... args) { return std::make_unique<T>(std::forward<Args>(args)...); }

		template<typename... Args>
		static Traits<Object>::Ptr make(Args&&... args) { return std::make_unique<Object>(std::forward<Args>(args)...); }

	private:

		Traits<Material>::SPtr m_material = nullptr;
		Traits<Mesh>::SPtr m_mesh = nullptr;

		Transform m_transform;

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
		virtual ~Light() = default;

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


	export class Scene
	{
	public:

		Scene(std::string_view name = "") : m_name(name) {}
		virtual ~Scene() = default;

		// Optional overrides
		virtual void onUpdate(float deltaTime) {}
		virtual void onEvent(event::Event& event) {}

		void addObject(Traits<Object>::Ptr obj)
		{
			m_objects.push_back(std::move(obj));
		}

		void addLight(Traits<Light>::Ptr light)
		{
			m_lights.push_back(std::move(light));
		}

		const std::vector<Traits<Object>::Ptr>& getObjects() const { return m_objects; }
		std::vector<Traits<Object>::Ptr>& getObjects() { return m_objects; }

		const std::vector<Traits<Light>::Ptr>& getLights() const { return m_lights; }
		std::vector<Traits<Light>::Ptr>& getLights() { return m_lights; }

		const std::string& getName() const { return m_name; }

		template<typename S, typename... Args>
			requires std::is_base_of_v<Scene, S>
		static Traits<S>::Ptr make(Args&&... args) { return std::make_unique<S>(std::forward<Args>(args)...); }

		static Traits<Scene>::Ptr make() { return std::make_unique<Scene>(); }

	private:

		std::string m_name;
		std::vector<Traits<Object>::Ptr> m_objects;
		std::vector<Traits<Light>::Ptr> m_lights;

	};

}