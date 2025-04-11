#include "Application.h"
#include <string>

namespace RGS {
	Application::Application(const char* name, int width, int height) : m_name(name), m_width(width), m_height(height) {
		Init();
	}
	Application::~Application() {

	}
	void Application::Run() {
		while (!m_Window->Closed()) {
			OnUpdate();
		}
	}
	void  Application::Init() {
		Window::Init();
		m_Window = Window::Create(m_name, m_width, m_height);
	}
	void  Application::Terminate() {
		delete m_Window;
		Window::Terminate();
	}
	void  Application::OnUpdate() {

	}
}