#pragma once 
#include "\cpp\CppResearch\rgs\Window\Window.h"
#include "\cpp\CppResearch\rgs\Window\Framebuffer.h"
#include "\cpp\CppResearch\rgs\Base\Maths.h"
#include "\cpp\CppResearch\rgs\Shader\BlinnShader.h"
#include "\cpp\CppResearch\rgs\Window\Renderer.h"
#include <vector>
#include <string>
#include <memory>
#include <chrono>

namespace RGS {
	struct Camera {
		Vec4 Pos = { 0.0f,0.0f,1.0f,1.0f };
		Vec4 Right = { 1.0f,0.0f,0.0f,0.0f };
		Vec4 Up = { 0.0f,1.0f,0.0f,0.0f };
		Vec4 Dir = { 0.0f,0.0f,-1.0f,0.0f };
		float Aspect = 4.0f / 4.0f;
	};

	class Application {
	public:
		Application(const char* name, int width, int length);
		~Application();
		void Run();
	private:
		void Init();
		void Terminate();
		void OnCameraUpdate(float time);
		void OnUpdate(float time);
		void LoadMesh(const char* fileName);
		const char* m_name;
		int m_width;
		int m_height;
		std::chrono::steady_clock::time_point m_LastFrameTime;
		Window* m_Window;
		Camera m_Camera;
		std::vector<Triangle<BlinnVertex>> m_Mesh;
	};
}