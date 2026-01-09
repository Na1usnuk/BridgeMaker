export module bm.app;

import std;

export import :controller;

import bm.gfx;
import bm.platform;
import bm.core;


namespace bm
{
	export class Application
	{
	public:

		int run(int argc, char** argv);
		int run();

		float getFPS() const { return m_timestep.getFPS(); }
		void setTargetFPS(unsigned short fps_limit) noexcept { m_timestep.setFPSLimit(fps_limit); }
		bool isOpen() const noexcept { return m_is_running; }

		Application(const Application&) = delete;
		Application& operator=(const Application&) = delete;
		Application(Application&&) = default;
		Application& operator=(Application&&) = default;

	protected:

		Application(std::string_view title = "Bridge Maker App", int width = 1280, int heigth = 720, bool decorated = true, bool visible = true);

		virtual ~Application() = default;

		void setReturnCode(int return_code) { m_return_code = return_code; }

		virtual void onArguments(int argc, char** argv) {}
		virtual void onStartup() {}
		virtual void onUpdate(float delta_time) = 0;
		virtual void onRender() {}
		//virtual void onEvent(event::Event& e);
		virtual void onShutdown() {}

		Window& getWindow() { return m_window; }

		void close() noexcept { m_is_running = false; }

	private:

		DeltaTime m_timestep;

		Window m_window;
		gfx::Context m_ctx;

		bool m_is_running;
		int m_return_code;
	};

}

