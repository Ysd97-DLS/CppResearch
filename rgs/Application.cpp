#include "Application.h"
#include <string>
#include <iostream>

namespace RGS {
	Application::Application(const char* name, int width, int height) : m_name(name), m_width(width), m_height(height) {
		Init();
	}
	Application::~Application() {

	}
	void Application::Run() {
		while (!m_Window->Closed()) {
			OnUpdate();
			Window::PollInputEvents();
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
		if (m_Window->GetKey(RGS_KEY_0) == RGS_PRESS) {
			std::cout << "0 was pressed" << std::endl;
		}
		if (m_Window->GetKey(RGS_KEY_A) == RGS_PRESS) {
			std::cout << "A was pressed" << std::endl;
		}
		Framebuffer framebuffer(m_width, m_height);
		framebuffer.Clear({ 1.0f,1.0f,1.0f });
		framebuffer.SetColor(100, 100, { 1.0f,1.0f,1.0f });
		m_Window->DrawFrambuffer(framebuffer);
	}
}