#include "core.hpp"

#include "window.hpp"

struct GLFWwindow;

BM_START


class WindowsWindow : public AbstractWindow
{
public:

	WindowsWindow(const Data& data);
	~WindowsWindow();

	void onUpdate() override;

	unsigned int getWidth() const override { return m_data.width; }
	unsigned int getHeight() const override { return m_data.height; }
	bool getVSync() const override { return m_data.vsync; };

	inline void setVSync(bool enabled) override;
	void resize(int width, int height) override;

	void init(const Data& props);

	inline bool isOpen() const override;
	inline void close() override;

private:

	GLFWwindow* m_window;

};

BM_END