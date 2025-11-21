export module sandbox;

export import BridgeMaker;

using namespace bm;
using namespace bm::gfx;

import scenes;


export class SandBox : public bm::Application
{
public:

	SandBox() : bm::Application("SandBox", 1200, 720), 
		m_camera(bm::gfx::Camera::make({0.f, 2.f, -3.f})), 
		m_scene(scene::triangle()),
		m_camera2d(bm::gfx::ScreenCamera::make(0, 1200, 0, 720))
	{
		setFPSLimit(120);


		m_camera_input = pushOverlay(Layer::make<DefaultCameraInputLayer>(m_camera.get()));
		m_camera_input->setSensetivity(0.2f);
		m_camera_input->setSpeed(5.f);

		getScreenRenderer().setCamera(m_camera2d);

		//m_camera->setAspectRatio(1200.f / 720.f);

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
		d.dispatch<bm::WindowResizeEvent>([this](bm::WindowResizeEvent& e) 
			{ 
				this->getRenderer().setView({0, 0, e.getWidth(), e.getHeight()}); 
				m_camera->setAspectRatio((float)e.getWidth() / (float)e.getHeight());
				return true; 
			});

		d.dispatch<bm::KeyPressEvent>(bm::bindEventFn(&SandBox::onKeyPress, this));
	}

	bool onRender(bm::AppRenderEvent& e)
	{	
		getRenderer().clear();
		getRenderer().draw(m_scene, m_camera);

		//using namespace bm::gfx;
		//static auto triangle = Object::make<Triangle>();

		// Test 1: Simple colored quad (no texture)
		getScreenRenderer().submit(
			glm::vec3(400.f, 300.f, 0.f),  // center of screen
			glm::vec2(100.f, 100.f),        // 100x100 size
			glm::vec4(1.f, 0.f, 0.f, 1.f)   // red color
		);

		// Test 2: Another quad at different position
		getScreenRenderer().submit(
			glm::vec2(200.f, 150.f),        // using vec2 constructor
			glm::vec2(50.f, 50.f),          // smaller quad
			glm::vec4(0.f, 1.f, 0.f, 1.f)   // green
		);

		getScreenRenderer().draw();

		return true;
	}

	void onImGuiRender() override
	{
		//static glm::vec3 position(0.f, 0.f, 0.f);
		//static glm::vec3 rotation(0.f, 0.f, 0.f);
		//static glm::vec3 scale(1.f, 1.f, 1.f);
		//static glm::vec3 color(1.f, 1.f, 1.f);

		//ImGui::Begin("Settings");

		//ImGui::DragFloat3("Position", &position.x, 0.1f);

		//ImGui::DragFloat3("Rotation", &rotation.x, 0.5f);

		//ImGui::DragFloat3("Scale", &scale.x, 0.05f, 0.01f, 100.0f);

		//ImGui::ColorEdit3("Color", &color.x);

		//ImGui::End();

		//m_scene->getObjects()[0]->setPosition(position);
		//m_scene->getObjects()[0]->setRotation(rotation);
		//m_scene->getObjects()[0]->setScale(scale);
		//m_scene->getObjects()[0]->setColor({ color.x, color.y, color.z, 1.f });
		//m_scene->getObjects()[0]->apply();
	}

	void onUpdate(float delta_time)
	{
	}

private:

	Traits<DefaultCameraInputLayer>::OPtr m_camera_input;
	Traits<Camera>::Ptr m_camera;
	Traits<ScreenCamera>::Ptr m_camera2d;

	Traits<Scene>::Ptr m_scene;

};
