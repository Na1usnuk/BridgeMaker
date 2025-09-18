export module sandbox;

export import BridgeMaker;

import scenes;


export class SandBox : public bm::Application
{
public:

	SandBox() : bm::Application("SandBox", 1200, 720), m_camera(bm::gfx::Camera::make({0.f, 2.f, -3.f})), m_scene(scene::cube())
		//m_camera2d(0, 1200, 0, 720)
	{
		setFPSLimit(120);


		auto camera_input = bm::Layer::make<bm::DefaultCameraInputLayer>(m_camera);
		camera_input->setSensetivity(0.2f);
		pushOverlay(camera_input);

		m_camera_input = camera_input;

		m_camera->setAspectRatio(1200.f / 720.f);
		m_camera->recalculateProjection();

		getRenderer().setDepthTesting(true);
		//getRenderer().setBlend(true);
		getRenderer().setBackgroundColor({0.3f, 0.3f, 0.3f, 1.0f});

	}

	bool onKeyPress(bm::KeyPressEvent& e)
	{
		if (e.getKey() == bm::Input::Key::ESCAPE)
		{
			close();
			return true;
		}
		return false;
	}

	void onEvent(bm::Event& e) 
	{
		bm::EventDispatcher d(e);

		d.dispatch<bm::AppRenderEvent>(bm::bindEventFn(& SandBox::onRender, this));

		d.dispatch<bm::WindowCloseEvent>([this](bm::WindowCloseEvent& e) { this->close(); return true; });
		d.dispatch<bm::WindowResizeEvent>([this](bm::WindowResizeEvent& e) { this->getRenderer().setView({0, 0, e.getWidth(), e.getHeight()}); return true; });

		d.dispatch<bm::KeyPressEvent>(bm::bindEventFn(&SandBox::onKeyPress, this));
	}

	bool onRender(bm::AppRenderEvent& e)
	{	
		getRenderer().clear();
		float aspect_ratio = (float)getRenderer().getView()[2] / (float)getRenderer().getView()[3];

		getRenderer().draw(m_scene, m_camera);

		return true;
	}

	void onImGuiRender() override
	{
		static glm::vec3 position(0.f, 0.f, 0.f);
		static glm::vec3 rotation(0.f, 0.f, 0.f);
		static glm::vec3 scale(1.f, 1.f, 1.f);
		static glm::vec3 color(1.f, 1.f, 1.f);

		ImGui::Begin("Settings");

		ImGui::DragFloat3("Position", &position.x, 0.1f);

		ImGui::DragFloat3("Rotation", &rotation.x, 0.5f);

		ImGui::DragFloat3("Scale", &scale.x, 0.05f, 0.01f, 100.0f);

		ImGui::ColorEdit3("Color", &color.x);

		ImGui::End();

		m_scene->getObjects()[0]->setPosition(position);
		m_scene->getObjects()[0]->setRotation(rotation);
		m_scene->getObjects()[0]->setScale(scale);
		m_scene->getObjects()[0]->setColor({ color.x, color.y, color.z, 1.f });
		m_scene->getObjects()[0]->apply();
	}

	void onUpdate(float delta_time)
	{
	}

private:

	std::weak_ptr<bm::DefaultCameraInputLayer> m_camera_input;
	bm::gfx::CameraPtr m_camera;
	bm::gfx::ScenePtr m_scene;
	//bm::gfx::Camera2D m_camera2d;

};
