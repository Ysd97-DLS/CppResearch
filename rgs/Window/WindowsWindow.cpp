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
<<<<<<< HEAD
		WNDCLASS wc = { 0 };
		wc.cbClsExtra = 0;//类型:int,要按照窗口类结构分配的额外字节数。 系统将字节初始化为零。
		wc.cbWndExtra = 0;//类型:int,在窗口实例之后分配的额外字节数。 系统将字节初始化为零。
		wc.hbrBackground = (HBRUSH)(WHITE_BRUSH);//（背景色）类型:HBRUSH,类背景画笔的句柄。 此成员可以是用于绘制背景的物理画笔的句柄,也可以是颜色值。
		wc.hCursor = NULL;//（默认光标）类型：HCURSOR,类游标的句柄。 此成员必须是游标资源的句柄。 如果此成员 NULL，则每当鼠标移动到应用程序的窗口中时，应用程序都必须显式设置光标形状。
		wc.hIcon = NULL;//（默认图标）类型：HICON,类图标的句柄。 此成员必须是图标资源的句柄。 如果此成员 NULL，则系统提供默认图标。
		wc.hInstance = GetModuleHandle(NULL);//类型：HINSTANCE,包含类的窗口过程的实例的句柄。
		wc.lpfnWndProc = WindowsWindow::WndProc;//（窗口处理函数）类型：WNDPROC,指向窗口过程的指针。 必须使用 CallWindowProc 函数来调用窗口过程。
		wc.lpszClassName = RGS_WINDOW_CLASS_NAME;//类型：LPCTSTR,指向以 null 结尾的字符串或原子的指针。
		wc.style = CS_HREDRAW | CS_VREDRAW;//（拉伸时重绘）类型：UINT,类样式（s）。 此成员可以是 类样式的任意组合。
		wc.lpszMenuName = NULL;//（ 无菜单）类型：LPCTSTR,类菜单的资源名称
		atom = RegisterClass(&wc);//（注册窗口）
	}
	void WindowsWindow::Unregister() {
		UnregisterClass(RGS_WINDOW_CLASS_NAME, GetModuleHandle(NULL));
	}
	LRESULT CALLBACK WindowsWindow::WndProc(const HWND hWnd, const UINT msgID, const WPARAM wParam, const LPARAM lParam) {

	};
	void WindowsWindow::Show() {
		
	}
	WindowsWindow::WindowsWindow(const std::string title, const int width, const int height) :Window(title, width, height) {
		ASSERT((s_Inited), "Not Initialized");
=======
>>>>>>> e250d3264507fd14ff6210f12dfccfcc73a2e332

	}
}