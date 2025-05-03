#pragma once
#include "Window.h"
#include <string>
#include <windows.h>

namespace RGS {
	class WindowsWindow : public Window {
	public:
		WindowsWindow(const char* title, const int width, const int height);
		~WindowsWindow();
		void Show() override;
		virtual void DrawFramebuffer(const Framebuffer& framebuffer) override;
		static void Init();
		static void Terminate();
		static void PollInputEvents();
	private:
		static void Register();
		static void Unregister();
		static void KeyPressImpl(WindowsWindow* Window, const WPARAM wParam, const char state);
		static LRESULT CALLBACK WndProc(const HWND hWnd, const UINT msgID, const WPARAM wParam, const LPARAM lParam);
		/*LRESULT 是程序返回到 Windows 的整数值。 它包含程序对特定消息的响应。 
		此值的含义取决于消息代码。 CALLBACK 是函数的调用约定。
		hwnd 是窗口的句柄;
		uMsg 是消息代码;
		wParam 和 lParam 包含与消息相关的其他数据。 具体含义依赖于消息代码。
		*/
		HWND m_Handle;
		HDC m_MemoryDC;
		unsigned char* m_Buffer;
		static bool s_Inited;
		
	};
}