#include "Application.h"
#include <string>
#include "Window.h"

namespace RGS {
	Application::Application(const std::string name, int width, int length) : m_name(name), m_width(width), m_length(length) {
		Init();
	}
	Application::~Application() {

	}
	void Application::Run() {
		bool running = true;
		if (running) {
			OnUpdate();
		}
	}
	void  Application::Init() {
		Window::Init();
	}
	void  Application::Terminate() {
		Window::Terminate();
	}
	void  Application::OnUpdate() {

	}
}