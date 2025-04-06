#pragma once
#include "Window.h"
#include <string>

namespace RGS {
	class WindowsWindow : public Window {
	public:
		WindowsWindow(const std::string title, const int width, const int height);
		~WindowsWindow();
		virtual void Show() const override;
		static void Init();
		static void Terminate();
	private:
		static void Register();
		static void Unregister();

		
	};
}