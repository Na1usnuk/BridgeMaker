export module bm.app;

import std;

export import :controller;
export import :event;

import bm.gfx;
import bm.platform;
import bm.core;

namespace bm::app
{
	export class Application
	{
	public:

		int run(int argc, char** argv);
		int run();

		//float getFPS() const { return m_timestep.getFPS(); }
		//void setTargetFPS(unsigned short fps_limit) noexcept { m_timestep.setFPSLimit(fps_limit); }
		bool isOpen() const noexcept { return m_is_running; }

		Application(const Application&) = delete;
		Application& operator=(const Application&) = delete;
		Application(Application&&) = default;
		Application& operator=(Application&&) = default;

	protected:

		Application(std::string_view title = "Bridge Maker App", int width = 1280, int height = 720, bool decorated = true, bool visible = true);

		virtual ~Application() = default;

		virtual void onArguments(int argc, char** argv) {}
		virtual void onStartup() {}
		virtual void onUpdate(float delta_time) = 0;
		virtual void onRender() {}
		virtual void onEvent(Event& e);
		virtual void onShutdown() {}

		void close() noexcept { m_is_running = false; }

	protected:

		Window window;
		gfx::Context graphic_context{ window };
		EventSystem event_system{ window };

		int return_code = 0;

	private:

		//DeltaTime m_timestep;

		bool m_is_running = true;

	};

}

