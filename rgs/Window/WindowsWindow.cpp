#include "\cpp\CppResearch\rgs\Base\Base.h"
#include "Window.h"
#include "WindowsWindow.h"

#include <windows.h>

#define RGS_WINDOW_ENTRY_NAME "Entry"
#define RGS_WINDOW_CLASS_NAME "Class"

namespace RGS {
	bool WindowsWindow::s_Inited = false;
	void WindowsWindow::Init() {
		ASSERT(!s_Inited);
		Register();
		s_Inited = true;
	};
	void WindowsWindow::Terminate() {
		ASSERT(s_Inited);
		Unregister();
		s_Inited = false;
	};
	void WindowsWindow::Register() {
		ATOM atom;
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
	void WindowsWindow::KeyPressImpl(WindowsWindow* Window, const WPARAM wParam, const char state) {
		if (wParam >= '0' && wParam <= '9') {
			Window->m_Keys[wParam] = state;
			return;
		}
		if (wParam >= 'A' && wParam <= 'Z'){
			Window->m_Keys[wParam] = state;
			return;
		}
		switch (wParam) {
		case VK_SPACE://按下空格
			Window->m_Keys[RGS_KEY_SPACE] = state;
			break;
		case VK_SHIFT://按下Shift
			Window->m_Keys[RGS_KEY_LEFT_SHIFT] = state;
			break;
		default:
			break;
		}
		//对键盘上按下的键进行设置
	}
	void WindowsWindow::PollInputEvents() {
		MSG message;
		while (PeekMessage(&message, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
	}
	LRESULT CALLBACK WindowsWindow::WndProc(const HWND hWnd, const UINT msgID, const WPARAM wParam, const LPARAM lParam) {
		WindowsWindow* window = (WindowsWindow*)GetProp(hWnd, RGS_WINDOW_ENTRY_NAME);
		if (window == NULL) {
			return DefWindowProc(hWnd, msgID, wParam, lParam);
		}
		switch (msgID) {
		case WM_DESTROY:
			window->m_Closed = true;
			return 0;
		case WM_KEYDOWN:
			KeyPressImpl(window, wParam, RGS_PRESS);
			return 0;
		case WM_KEYUP:
			KeyPressImpl(window, wParam, RGS_RELEASE);
			return 0;
		}
		return DefWindowProc(hWnd, msgID, wParam, lParam);
	};
	void WindowsWindow::Show() {
		HDC windowDC = GetDC(m_Handle);
		BitBlt(windowDC, 0, 0, m_Width, m_Height, m_MemoryDC, 0, 0, SRCCOPY);
		ShowWindow(m_Handle, SW_SHOW);
		ReleaseDC(m_Handle, windowDC);
	}
	WindowsWindow::WindowsWindow(const char* title, const int width, const int height) :Window(title, width, height) {
		ASSERT((s_Inited), "Not Initialized");
		DWORD style = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
		RECT rect;
		rect.left = 0;
		rect.top = 0;
		rect.bottom = (long)height;
		rect.right = (long)width;
		AdjustWindowRect(&rect, style, false);
		m_Handle = CreateWindow(RGS_WINDOW_CLASS_NAME, m_Title, style, CW_USEDEFAULT, 0, rect.right - rect.left, rect.bottom - rect.top, NULL, NULL, GetModuleHandle(NULL), NULL);
		ASSERT(m_Handle != NULL);
		m_Closed = false;
		SetProp(m_Handle, RGS_WINDOW_ENTRY_NAME, this);

		HDC windowDC = GetDC(m_Handle);
		m_MemoryDC = CreateCompatibleDC(windowDC);
		BITMAPINFOHEADER biHeader = {};
		HBITMAP newBitmap;
		HBITMAP oldBitmap;
		biHeader.biSize = sizeof(BITMAPINFOHEADER);
		biHeader.biWidth = ((long)m_Width);
		biHeader.biHeight= -((long)m_Height);//让位图从左上角开始
		biHeader.biPlanes = 1;
		biHeader.biBitCount = 24;
		biHeader.biCompression = BI_RGB;
		//分配空间
		newBitmap = CreateDIBSection(m_MemoryDC, (BITMAPINFO*)&biHeader, DIB_RGB_COLORS, (void**)&m_Buffer, nullptr, 0);
		ASSERT(newBitmap != NULL);
		constexpr int channelCount = 3;
		int size = m_Width * m_Height * channelCount * sizeof(unsigned char);
		memset(m_Buffer, 0, size);
		oldBitmap = (HBITMAP)SelectObject(m_MemoryDC, newBitmap);

		DeleteObject(oldBitmap);
		ReleaseDC(m_Handle, windowDC);

		Show();
	}
	WindowsWindow::~WindowsWindow() {
		ShowWindow(m_Handle, SW_HIDE);
		RemoveProp(m_Handle, RGS_WINDOW_ENTRY_NAME);
		DeleteDC(m_MemoryDC);
		DestroyWindow(m_Handle);
	}
	void WindowsWindow::DrawFramebuffer(const Framebuffer& framebuffer) {
		const int fWidth = framebuffer.GetWidth();
		const int fHeight = framebuffer.GetHeight();
		const int width = m_Width < fWidth ? m_Width : fWidth;
		const int height = m_Height < fHeight ? m_Height : fHeight;
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				constexpr int channelCount = 3;
				constexpr int rChannel = 2;
				constexpr int gChannel = 1;
				constexpr int bChannel = 0;
				Vec3 color = framebuffer.GetColor(j, fHeight - 1 - i);
				const int pixStart = (i * m_Width + j) * channelCount;
				const int rIdx = pixStart + rChannel;
				const int gIdx = pixStart + gChannel;
				const int bIdx = pixStart + bChannel;
				m_Buffer[rIdx] = Float_UChar(color.x);
				m_Buffer[gIdx] = Float_UChar(color.y);
				m_Buffer[bIdx] = Float_UChar(color.z);
			}
		}
		Show();
	}
}