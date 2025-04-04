#include "Application.h"
#include <string>

namespace RGS {
	class Application {
	public:
		Application(const std::string name, int width, int length) {
			m_name = name;
			m_width = width;
			m_length = length;
		}
		~Application() {

		}
		void Run() {
			bool running = true;
			if (running) {
				OnUpdate();
			}
		}
	private:
		void Init() {

		}
		void Terminate() {

		}
		void OnUpdate() {

		}
		std::string m_name;
		int m_width;
		int m_length;
	};
}