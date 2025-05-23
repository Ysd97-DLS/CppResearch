#pragma once 
#include "../rgs/Window/Window.h"
#include "../rgs/Window/Framebuffer.h"
#include "../rgs/Base/Maths.h"
#include "../rgs/Shader/BlinnShader.h"
#include "../rgs/Scene/Scene.h"
#include "../rgs/Scene/MeshObject.h"
#include <vector>
#include <string>
#include <memory>
#include <chrono>
#include <thread>
#include <mutex>

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
        Scene& GetScene() { return m_Scene; }
        const Scene& GetScene() const { return m_Scene; }
        
        void SetModelPath(const std::string& path) { m_ModelPath = path; }
        void SetTexturePath(const std::string& path) { m_TexturePath = path; }
        
        void SetLightIntensity(float intensity) { m_LightIntensity = intensity; }

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
        Scene m_Scene;
        std::string m_ModelPath;
        std::string m_TexturePath;
        float m_LightIntensity = 1.0f;
    private:
        void RenderObject(const std::shared_ptr<MeshObject>& meshObject, 
                         Framebuffer& framebuffer,
                         const Program<BlinnVertex, BlinnUniforms, BlinnVaryings>& program,
                         const Mat& view,
                         const Mat& proj);
        
        std::mutex m_FramebufferMutex;
        int m_ThreadCount = std::thread::hardware_concurrency();
    };
}