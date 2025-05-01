#include "Application.h"
#include "\cpp\CppResearch\rgs\Window\Framebuffer.h"
#include "\cpp\CppResearch\rgs\Base\Maths.h"
#include "\cpp\CppResearch\rgs\Shader\BlinnShader.h"
#include "\cpp\CppResearch\rgs\Window\Renderer.h"
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
	void Application::Init() {
		Window::Init();
		m_Window = Window::Create(m_name, m_width, m_height);
	}
	void Application::Terminate() {
		delete m_Window;
		Window::Terminate();
	}
	void Application::OnUpdate() {
		if (m_Window->GetKey(RGS_KEY_0) == RGS_PRESS) {
			std::cout << "0 was pressed" << std::endl;
		}
		if (m_Window->GetKey(RGS_KEY_A) == RGS_PRESS) {
			std::cout << "A was pressed" << std::endl;
		}
		Framebuffer framebuffer(m_width, m_height);
		framebuffer.Clear();

		Program program(BlinnVertexShader, BlinnFragmentShader);
		Triangle<BlinnVertex> triangle;
		triangle[0].ModelPos = { 0.0f,0.0f,-10.0f,1.0f };
		triangle[1].ModelPos = { -10.0f,-10.0f,-10.0f,1.0f };
		triangle[2].ModelPos = { 30.0f,-10.0f,-10.0f,1.0f };
		BlinnUniforms uniforms;
		uniforms.mvp = Perspective(90.0f / 180.0f * PI, 1.0f, 1.0f, 10.0f);
		Renderer::Draw(framebuffer, program, triangle, uniforms);
		m_Window->DrawFrambuffer(framebuffer);
	}
}