#include "Window.h"
#include "WindowsWindow.h"

namespace RGS {
	Window::Window(std::string title, const int width, const int height):m_Title(title), m_Width(width), m_Height(height), m_Closed(true) {

	}
}