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
		wc.cbClsExtra = 0;//����:int,Ҫ���մ�����ṹ����Ķ����ֽ����� ϵͳ���ֽڳ�ʼ��Ϊ�㡣
		wc.cbWndExtra = 0;//����:int,�ڴ���ʵ��֮�����Ķ����ֽ����� ϵͳ���ֽڳ�ʼ��Ϊ�㡣
		wc.hbrBackground = (HBRUSH)(WHITE_BRUSH);//������ɫ������:HBRUSH,�౳�����ʵľ���� �˳�Ա���������ڻ��Ʊ����������ʵľ��,Ҳ��������ɫֵ��
		wc.hCursor = NULL;//��Ĭ�Ϲ�꣩���ͣ�HCURSOR,���α�ľ���� �˳�Ա�������α���Դ�ľ���� ����˳�Ա NULL����ÿ������ƶ���Ӧ�ó���Ĵ�����ʱ��Ӧ�ó��򶼱�����ʽ���ù����״��
		wc.hIcon = NULL;//��Ĭ��ͼ�꣩���ͣ�HICON,��ͼ��ľ���� �˳�Ա������ͼ����Դ�ľ���� ����˳�Ա NULL����ϵͳ�ṩĬ��ͼ�ꡣ
		wc.hInstance = GetModuleHandle(NULL);//���ͣ�HINSTANCE,������Ĵ��ڹ��̵�ʵ���ľ����
		wc.lpfnWndProc = WindowsWindow::WndProc;//�����ڴ����������ͣ�WNDPROC,ָ�򴰿ڹ��̵�ָ�롣 ����ʹ�� CallWindowProc ���������ô��ڹ��̡�
		wc.lpszClassName = RGS_WINDOW_CLASS_NAME;//���ͣ�LPCTSTR,ָ���� null ��β���ַ�����ԭ�ӵ�ָ�롣
		wc.style = CS_HREDRAW | CS_VREDRAW;//������ʱ�ػ棩���ͣ�UINT,����ʽ��s���� �˳�Ա������ ����ʽ��������ϡ�
		wc.lpszMenuName = NULL;//�� �޲˵������ͣ�LPCTSTR,��˵�����Դ����
		atom = RegisterClass(&wc);//��ע�ᴰ�ڣ�
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