#include "core/core.hpp"

#include "core/window.hpp"

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

	void setVSync(bool enabled) override;

	void init(const Data& props);

	bool isOpen() const override;
	void close() override;

private:

	GLFWwindow* m_window;
	Data m_data;

};

BM_END