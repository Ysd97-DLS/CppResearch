#pragma once
#include "\cpp\CppResearch\rgs\Base\Base.h"
#include "Framebuffer.h"

namespace RGS {
	Framebuffer::Framebuffer(const int width, const int height) :m_Width(width), m_Height(height) {
		ASSERT((width > 0) && (height > 0));
		m_PixelSize = m_Width * m_Height;
		m_ColorBuffer = new Vec3[m_PixelSize]();
		m_DepthBuffer = new float[m_PixelSize]();
		Clear({ 0.0f,0.0f,0.0f });
		ClearDepth(1.0f);
	}
	void Framebuffer::Clear(const Vec3& color) {
		for (int i = 0; i < m_PixelSize; i++) {
			m_ColorBuffer[i] = color;
		}
	}
	void Framebuffer::ClearDepth(const float depth) {
		for (int i = 0; i < m_PixelSize; i++) {
			m_DepthBuffer[i] = depth;
		}
	}
	void Framebuffer::SetColor(const int x, const int y, const Vec3& color) {
		if ((x < 0) || (x >= m_Width) || (y < 0) || (y >= m_Height)) {
			//ASSERT(false);
			return;
		}
		int idx = GetPixelIdx(x, y);
		if (idx < m_PixelSize && idx >= 0) {
			m_ColorBuffer[idx] = color;
		}
		else {
			ASSERT(false);
		}
	}
	Vec3 Framebuffer::GetColor(const int x, const int y)const {
		int idx = GetPixelIdx(x, y);
		if (idx < m_PixelSize && idx >= 0) {
			return m_ColorBuffer[idx];
		}
		else {
			ASSERT(false);
			return { 0.0f,0.0f,0.0f };
		}
	}
	void Framebuffer::SetDepth(const int x, const int y, const float depth) {
		int idx = GetPixelIdx(x, y);
		if (idx < m_PixelSize && idx >= 0) {
			m_DepthBuffer[idx] = depth;
		}
		else {
			ASSERT(false);
		}
	}
	float Framebuffer::GetDepth(const int x, const int y)const {
		int idx = GetPixelIdx(x, y);
		if (idx < m_PixelSize && idx >= 0) {
			return m_DepthBuffer[idx];
		}
		else {
			ASSERT(false);
			return { 1.0f };
		}
	}
	Framebuffer::~Framebuffer() {

	}
}