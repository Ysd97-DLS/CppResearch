#pragma once 
#include <iostream>

namespace RGS {
	class Application {
	public:
		Application(const std::string name, int width, int length);
		~Application();
		void Run();
	private:
		void Init();
		void Terminate();
		void OnUpdate();
		std::string m_name;
		int m_width;
		int m_length;
	};
}