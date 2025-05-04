#pragma once
#include <vector>
#include "../Base/Maths.h"

namespace RGS {
	class Texture {
	private:
		std::vector<Vec4> m_data;
		int m_width;
		int m_height;
	public:
		Texture(int width, int height) : m_width(width), m_height(height) {
			m_data.resize(width * height);
		}
		void SetPixel(int x, int y, const Vec4& color) {
			if (x >= 0 && x < m_width && y >= 0 && y < m_height) {
				m_data[y * m_width + x] = color;
			}
		}
		Vec4 Sample(float u, float v)const {
			float x = u * (m_width - 1);
			float y = v * (m_height - 1);
			int x0 = static_cast<int>(x);
			int y0 = static_cast<int>(y);
			int x1 = std::min(x0 + 1, m_width - 1);
			int y1 = std::min(y0 + 1, m_height - 1);
			float fx = x - x0;
			float fy = y - y0;
			Vec4 c00 = m_data[y0 * m_width + x0];
			Vec4 c10 = m_data[y0 * m_width + x1];
			Vec4 c01 = m_data[y1 * m_width + x0];
			Vec4 c11 = m_data[y1 * m_width + x1];
			//Vec4 c0 = Lerp(c00, c10, fx);
			//Vec4 c1 = Lerp(c10, c11, fx);
			//return Lerp(c0, c1, fy);
		}
		int GetWidth()const {
			return m_width;
		}
		int GetHeight()const {
			return m_height;
		}
	};
}