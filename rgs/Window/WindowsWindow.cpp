#include "Base.h"
#include "Window.h"
#include "WindowsWindow.h"

#include <windows.h>

#define RGS_WINDOW_ENTRY_NAME "Entry"
#define RGS_WINDOW_CLASS_NAME "Class"

namespace RGS {
	bool WindowsWindow::s_Inited = false;
	void WindowsWindow::Init() {};
	void WindowsWindow::Terminate() {};
	void WindowsWindow::Register() {
		ATOM atom;

	}
}