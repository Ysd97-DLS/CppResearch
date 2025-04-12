#pragma once
#include "InputCode.h"
#include "Framebuffer.h"

namespace RGS {
	class Window {
	public:
		Window(const char* title, const int width, const int height);
		virtual ~Window();
		virtual void Show() = 0;
		virtual void DrawFrambuffer(const Framebuffer& framebuffer) = 0;
		bool Closed() const {
			return m_Closed;
		}
		char GetKey(const unsigned int idx) const {
			return m_Keys[idx];
		}
		bool Minimized() const {
			return m_Minimized;
		}
		static void Init();
		static void PollInputEvents() ;
		static void Terminate();
		static Window* Create(const char* title, const int width, const int height);
	protected:
		const char* m_Title;
		int m_Width;
		int m_Height;
		bool m_Closed;
		bool m_Minimized;
		char m_Keys[RGS_KEY_MAX_COUNT];
	};
}