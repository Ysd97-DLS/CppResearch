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
		/*LRESULT �ǳ��򷵻ص� Windows ������ֵ�� ������������ض���Ϣ����Ӧ�� 
		��ֵ�ĺ���ȡ������Ϣ���롣 CALLBACK �Ǻ����ĵ���Լ����
		hwnd �Ǵ��ڵľ��;
		uMsg ����Ϣ����;
		wParam �� lParam ��������Ϣ��ص��������ݡ� ���庬����������Ϣ���롣
		*/
		HWND m_Handle;
		HDC m_MemoryDC;
		unsigned char* m_Buffer;
		static bool s_Inited;
		
	};
}