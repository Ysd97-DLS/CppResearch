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
		
		// 设置场景背景颜色
		m_Scene.SetBackgroundColor(Vec3(0.2f, 0.2f, 0.2f));
		
		// 选择纹理
		int textureChoice = 0;
		bool validChoice = false;
		do {
			std::cout << "\nChoose texture type:" << std::endl;
			std::cout << "1. square" << std::endl;
			std::cout << "2. metal" << std::endl;
			std::cout << "3. white" << std::endl;
			
			if (std::cin >> textureChoice && textureChoice >= 1 && textureChoice <= 3) {
				validChoice = true;
			} else {
				std::cout << "Invalid choice. Please try again.\n";
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			}
		} while (!validChoice);
		
		switch(textureChoice) {
			case 1:
				m_TexturePath = "../../../Assets/square.ppm";
				break;
			case 2:
				m_TexturePath = "../../../Assets/metal.ppm";
				break;
			case 3:
				m_TexturePath = "../../../Assets/white.ppm";
				break;
			default:
				m_TexturePath = "../../../Assets/square.ppm";
				break;
		}
		
		bool addingObjects = true;
		while (addingObjects) {
		    std::cout << "\nChoose addition setting:" << std::endl;
		    std::cout << "1.Add new object" << std::endl;
		    std::cout << "2. Addition finished" << std::endl;
		    
		    int choice;
		    std::cin >> choice;
		    
		    if (choice == 2) {
		        addingObjects = false;
		        continue;
		    }
		    
		    std::cout << "\nChoose model type:" << std::endl;
		    std::cout << "1. box" << std::endl;
		    std::cout << "2. sphere" << std::endl;
		    std::cout << "3. hornet" << std::endl;
		    
		    int modelChoice;
		    std::cin >> modelChoice;
		    
		    std::string modelPath;
		    switch (modelChoice) {
		    case 1:
		        modelPath = "../../../Assets/box.obj";
		        break;
		    case 2:
		        modelPath = "../../../Assets/sphere.obj";
		        break;
		    case 3:
		        modelPath = "../../../Assets/hornet.obj";
		        break;
		    default:
		        modelPath = "../../../Assets/box.obj";
		        break;
		    }
		    auto meshObject = std::make_shared<MeshObject>("Object_" + std::to_string(m_Scene.GetObjects().size() + 1));
		    meshObject->LoadFromFile(modelPath.c_str());
		    meshObject->SetTexturePath(m_TexturePath.empty() ? "../../../Assets/square.ppm" : m_TexturePath);

		    float x, y, z;
		    std::cout << "\nPress object position (x y z): ";
		    std::cin >> x >> y >> z;
		    meshObject->SetPosition(Vec3(x, y, z));
		    
		    std::cout << "\nPress object rotation angle (x y z) [radian]: ";
		    std::cin >> x >> y >> z;
		    meshObject->SetRotation(Vec3(x, y, z));
		    
		    std::cout << "\nPress object scale (x y z): ";
		    std::cin >> x >> y >> z;
		    meshObject->SetScale(Vec3(x, y, z));
		    
		    m_Scene.AddObject(meshObject);
		}
		
		float lightIntensity = 1.0f;
		do {
		    std::cout << "\nChoose light intensity (0.0-128.0): ";
		    std::cin >> lightIntensity;
		} while (lightIntensity < 0.0f || lightIntensity > 128.0f);
		SetLightIntensity(lightIntensity);
	}
	void Application::Terminate() {
		delete m_Window;
		Window::Terminate();
	}
	void Application::OnUpdate(float time) {
		OnCameraUpdate(time);
		
		Framebuffer framebuffer(m_width, m_height);
		framebuffer.Clear(m_Scene.GetBackgroundColor());
		
		Program program(BlinnVertexShader, BlinnFragmentShader);
		Mat view = LookAt(m_Camera.Pos, m_Camera.Pos + m_Camera.Dir, { 0.0f,1.0f,0.0f });
		Mat proj = Perspective(90.0f / 360.0f * 2.0f * PI, m_Camera.Aspect, 0.1f, 100.0f);
		
		const auto& objects = m_Scene.GetObjects();
		std::vector<std::thread> threads;
		std::vector<std::shared_ptr<MeshObject>> meshObjects;
		
		for (const auto& object : objects) {
		    if (auto meshObject = std::dynamic_pointer_cast<MeshObject>(object)) {
		        meshObjects.push_back(meshObject);
		    }
		}
		
		int objectsPerThread = (meshObjects.size() + m_ThreadCount - 1) / m_ThreadCount;
		
		for (int i = 0; i < m_ThreadCount; ++i) {
		    int startIdx = i * objectsPerThread;
		    int endIdx = std::min(startIdx + objectsPerThread, static_cast<int>(meshObjects.size()));
		    
		    if (startIdx >= meshObjects.size()) break;
		    
		    threads.emplace_back([this, &framebuffer, &program, &view, &proj, &meshObjects, startIdx, endIdx]() {
		        for (int j = startIdx; j < endIdx; ++j) {
		            RenderObject(meshObjects[j], framebuffer, program, view, proj);
		        }
		    });
		}
		for (auto& thread : threads) {
		    thread.join();
		}
		
		m_Window->DrawFramebuffer(framebuffer);
	}
	void Application::RenderObject(const std::shared_ptr<MeshObject>& meshObject,
	                             Framebuffer& framebuffer,
	                             const Program<BlinnVertex, BlinnUniforms, BlinnVaryings>& program,
	                             const Mat& view,
	                             const Mat& proj) {
	    Mat model = meshObject->GetTransform();
	    BlinnUniforms uniform(meshObject->GetTexturePath());
	    uniform.CameraPos = m_Camera.Pos;
	    uniform.Model = model;
	    uniform.ModelNormalToWorld = Identity();
	    uniform.mvp = proj * view * model;
	    uniform.Shininess = m_LightIntensity;
	    
	    const auto& mesh = meshObject->GetMesh();
	    for (const auto& tri : mesh) {
	        std::lock_guard<std::mutex> lock(m_FramebufferMutex);
	        Renderer::Draw(framebuffer, program, tri, uniform);
	    }
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