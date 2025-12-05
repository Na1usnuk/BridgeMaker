export module sandbox;

export import BridgeMaker;

using namespace bm;
using namespace bm::gfx;

import scenes;

import std;


export class SandBox : public bm::Application
{
public:

	SandBox() : bm::Application("SandBox", 1200, 720), 
		m_camera(bm::gfx::Camera::make({0.f, 2.f, -3.f})), 
		m_scene(bm::gfx::Scene::make<scene::VRMLMaterialsScene>()),
		m_camera2d(bm::gfx::ScreenCamera::make(1200, 720))
	{
		setFPSLimit(120);


		m_camera_input = pushOverlay(Layer::make<DefaultCameraInputLayer>(m_camera.get()));
		m_camera_input->setSensetivity(0.2f);
		m_camera_input->setSpeed(5.f);

		getScreenRenderer().setCamera(m_camera2d);

		getRenderer().setDepthTesting(true);
		getRenderer().setBlend(true);
		getRenderer().setBlendFunc();
		//getRenderer().setBackgroundColor({ 0.53f, 0.81f, 0.92f, 1.0f });
		getRenderer().setBackgroundColor({ 0.f, 0.f, 0.f, 1.0f });

		getWindow().setCaptureCursor(true);

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
				m_camera2d->setViewportSize(e.getWidth(),e.getHeight());
				return true; 
			});

		d.dispatch<bm::KeyPressEvent>(bm::bindEventFn(&SandBox::onKeyPress, this));
	}

	bool onRender(bm::AppRenderEvent& e)
	{	
		getRenderer().clear();
		getRenderer().draw(m_scene, m_camera);

		auto [mouse_x, mouse_y] = bm::Input::getMousePosition();
		auto frame_y = getWindow().getFramebufferSize().second;

		getScreenRenderer().submit(
			glm::vec2(mouse_x, -(mouse_y - frame_y) - 50.f),      
			glm::vec2(50.f, 50.f),        
			bm::AssetManager::get().loadTexture("cursor", "res/cursor.png")
		);

		getScreenRenderer().draw();

		return true;
	}

	void onUpdate(float delta_time)
	{
		m_scene->onUpdate(delta_time);
	}

private:

	Traits<DefaultCameraInputLayer>::OPtr m_camera_input;
	Traits<Camera>::Ptr m_camera;
	Traits<ScreenCamera>::Ptr m_camera2d;

	Traits<Scene>::Ptr m_scene;

};
