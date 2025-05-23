#include "../Base/Base.h"
#include "Window.h"
#include "WindowsWindow.h"

namespace RGS {
	Window::Window(const char* title, const int width, const int height) :m_Title(title), m_Width(width), m_Height(height), m_Closed(true) {
		ASSERT((width > 0) && (height > 0));
		memset(m_Keys, RGS_RELEASE, RGS_KEY_MAX_COUNT);
	}
	void Window::Init() {
		WindowsWindow::Init();
	}
	void Window::Terminate() {
		WindowsWindow::Terminate();
	}
	Window* Window::Create(const char* title, const int width, const int height) {
		ASSERT((width > 0) && (height > 0));
		return new WindowsWindow(title, width, height);
	}
	Window::~Window() {

	}
	void Window::PollInputEvents() {
		WindowsWindow::PollInputEvents();
	}
}