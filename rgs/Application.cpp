#include "Application.h"
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

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
		LoadMesh("\\cpp\\CppResearch\\Assets\\sphere.obj");
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
		Mat view = LookAt(m_Camera.Pos, m_Camera.Pos + m_Camera.Dir, { 0.0f,1.0f,0.0f });
		Mat proj = Perspective(90.0f / 360.0f * 2.0f * PI, m_Camera.Aspect, 0.1f, 100.0f);
		Mat model = Identity();
		BlinnUniforms uniform;
		uniform.mvp = proj * view * model;
		uniform.CameraPos = m_Camera.Pos;
		uniform.Model = model;
		uniform.ModelNormalToWorld = Identity();
		for (auto tri : m_Mesh) {
			Renderer::Draw(framebuffer, program, tri, uniform);
		}
		m_Window->DrawFramebuffer(framebuffer);
	}
	void Application::LoadMesh(const char* fileName) {
		std::ifstream file(fileName, std::ios::in);
		ASSERT(file);
		std::vector<Vec3> positions;
		std::vector<Vec2> texCoords;
		std::vector<Vec3> normals;
		std::vector<int> posIndices;
		std::vector<int> texIndices;
		std::vector<int> normalIndices;
		std::string line;
		while (!file.eof()) {
			std::getline(file, line);
			int items = -1;
			if (line.find("v ") == 0) {
				Vec3 position;
				items = sscanf(line.c_str(), "v %f %f %f", &position.x, &position.y, &position.z);
				ASSERT(items == 3);
				positions.push_back(position);
			}
			else if (line.find("vt ") == 0) {
				Vec2 texCoord;
				items = sscanf(line.c_str(), "vt %f %f", &texCoord.x, &texCoord.y);
				ASSERT(items == 2);
				texCoords.push_back(texCoord);
			}
			else if (line.find("vn ") == 0) {
				Vec3 normal;
				items = sscanf(line.c_str(), "vn %f %f %f", &normal.x, &normal.y, &normal.z);
				ASSERT(items == 3);
				normals.push_back(normal);
			}
			else if (line.find("f ") == 0) {
				int pIndices[3], uvIndices[3], nIndices[3];
				items = sscanf(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d", &pIndices[0], &uvIndices[0], &nIndices[0], &pIndices[1], &uvIndices[1], &nIndices[1], &pIndices[2], &uvIndices[2], &nIndices[2]);
				ASSERT(items == 9);
				for (int i = 0; i < 3; i++) {
					posIndices.push_back(pIndices[i] - 1);
					texIndices.push_back(uvIndices[i] - 1);
					normalIndices.push_back(nIndices[i] - 1);
				}
			}
		}
		file.close();
		int triNum = posIndices.size() / 3;
		for (int i = 0; i < triNum; i++) {
			Triangle<BlinnVertex> triangle;
			for (int j = 0; j < 3; j++) {
				int index = 3 * i + j;
				int posIndex = posIndices[index];
				int texIndex = texIndices[index];
				int nIndex = normalIndices[index];
				triangle[j].ModelPos = { positions[posIndex],1.0f };
				triangle[j].TexCoord = texCoords[texIndex];
				triangle[j].ModelNormal = normals[nIndex];
			}
			m_Mesh.emplace_back(triangle);
		}
	}
}