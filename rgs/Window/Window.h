#pragma once

namespace RGS {
	class Window {
	public:
		Window(const char* title, const int width, const int height);
		virtual ~Window();
		virtual void Show() = 0;
		static void Init();
		static void Terminate();
	protected:
		const char* m_Title;
		int m_Width;
		int m_Height;
	};
}