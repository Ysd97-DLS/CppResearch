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
			auto nowFrameTime = std::chrono::steady_clock::now();
			auto duration = std::chrono::duration_cast<std::chrono::microseconds>(nowFrameTime - m_LastFrameTime);
			float deltaTime = duration.count() * 0.001f * 0.001f;
			m_LastFrameTime = nowFrameTime;
			OnUpdate(deltaTime);
			Window::PollInputEvents();
		}
	}
	void Application::OnCameraUpdate(float time) {
		constexpr float speed = 1.0f;
		if (m_Window->GetKey(RGS_KEY_SPACE) == RGS_PRESS)
			m_Camera.Pos = m_Camera.Pos + speed * time * m_Camera.Up;
		if (m_Window->GetKey(RGS_KEY_LEFT_SHIFT) == RGS_PRESS)
			m_Camera.Pos = m_Camera.Pos - speed * time * m_Camera.Up;
		if (m_Window->GetKey(RGS_KEY_D) == RGS_PRESS)
			m_Camera.Pos = m_Camera.Pos + speed * time * m_Camera.Right;
		if (m_Window->GetKey(RGS_KEY_A) == RGS_PRESS)
			m_Camera.Pos = m_Camera.Pos - speed * time * m_Camera.Right;
		if (m_Window->GetKey(RGS_KEY_W) == RGS_PRESS)
			m_Camera.Pos = m_Camera.Pos + speed * time * m_Camera.Dir;
		if (m_Window->GetKey(RGS_KEY_S) == RGS_PRESS)
			m_Camera.Pos = m_Camera.Pos - speed * time * m_Camera.Dir;

		constexpr float rotateSpeed = 1.0f;
		Mat rotation = Identity();
		if (m_Window->GetKey(RGS_KEY_Q) == RGS_PRESS)
			rotation = RotateY(time * rotateSpeed);
		if (m_Window->GetKey(RGS_KEY_E) == RGS_PRESS)
			rotation = RotateY(-time * rotateSpeed);
		m_Camera.Dir = rotation * m_Camera.Dir;
		m_Camera.Right = rotation * m_Camera.Right;
	}
	void Application::Init() {
		Window::Init();
		m_Window = Window::Create(m_name, m_width, m_height);
	}
	void Application::Terminate() {
		delete m_Window;
		Window::Terminate();
	}
	void Application::OnUpdate(float time) {
		OnCameraUpdate(time);
		Framebuffer framebuffer(m_width, m_height);
		framebuffer.Clear();

		Program program(BlinnVertexShader, BlinnFragmentShader);
		Triangle<BlinnVertex> triangle;
		triangle[0].ModelPos = { 0.0f,0.0f,-10.0f,1.0f };
		triangle[1].ModelPos = { -10.0f,-10.0f,-10.0f,1.0f };
		triangle[2].ModelPos = { 30.0f,-10.0f,-10.0f,1.0f };
		BlinnUniforms uniforms;
		Mat view = LookAt(m_Camera.Pos, m_Camera.Pos + m_Camera.Dir, { 0.0f,1.0f,0.0f });
		Mat proj = Perspective(90.0f / 360.0f * 2.0f * PI, m_Camera.Aspect, 0.1f, 100.0f);
		uniforms.mvp = proj * view;
		Renderer::Draw(framebuffer, program, triangle, uniforms);
		m_Window->DrawFramebuffer(framebuffer);
	}
}