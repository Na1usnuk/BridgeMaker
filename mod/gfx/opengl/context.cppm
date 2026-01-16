export module bm.gfx:context;

import bm.platform;
import bm.config;
import bm.core;

namespace bm::gfx
{
	export class Context
	{
	public:

		Context(bm::Window& window, Context& shared);
		Context(bm::Window& window);

		Version getVersion() const noexcept { return m_gl_version; }
		void swapBuffers() noexcept;
		void makeCurrent() noexcept;

		bool getVSync() const noexcept { return m_vsync; }
		void setVSync(bool enable) noexcept;

		static Context& getCurrent() noexcept { core::verify(s_current_ctx, "Current context is not set"); return *s_current_ctx; }

	private:

		void init();
		void init(bm::Window& window, Context& shared);
		void destroy();
		void infoPrint(); 
		void recognizeVersion();
		bm::Window& getWindow() const;
		void setVersion(Version version) { m_gl_version = version; }

	private:

		bm::Window* m_window;
		Version m_gl_version;
		bool m_vsync = true;
		static Context* s_current_ctx;

	};
}