#pragma once

namespace RGS {
	class Window {
	public:
		Window(const char* title, const int width, const int height);
		virtual ~Window();
		virtual void Show() = 0;
		bool Closed() const {
			return m_Closed;
		}
		static void Init();
		static void Terminate();
		static Window* Create(const char* title, const int width, const int height);
	protected:
		const char* m_Title;
		int m_Width;
		int m_Height;
		bool m_Closed;
		bool m_Minimized;
	};
}