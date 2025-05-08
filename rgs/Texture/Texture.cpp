#include "../Base/Base.h"
#include <algorithm>
#include "Texture.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

namespace RGS {
	Texture::Texture() : m_width(0), m_height(0), m_channels(0), m_data(nullptr) {
	}

	Texture::Texture(const std::string& path) 
	: m_path(path), m_width(0), m_height(0), m_channels(0), m_data(nullptr) {
	Init();
	}

	Texture::~Texture() {
		if (m_data)
			delete[] m_data;
		m_data = nullptr;
	}

	void Texture::Init() {
		std::ifstream file(m_path);
		if (!file.is_open()) {
			std::cerr << "Failed to open texture file: " << m_path << std::endl;
			return;
		}

		std::string line;
		// 读取文件头
		if (!std::getline(file, line)) {
			std::cerr << "Failed to read file header: " << m_path << std::endl;
			return;
		}
		
		if (line != "P3") {
			std::cerr << "Unsupported PPM format. Only P3 (ASCII) is supported." << std::endl;
			return;
		}

		// 跳过注释行
		while (std::getline(file, line) && line[0] == '#');

		std::istringstream iss(line);
		if (!(iss >> m_width >> m_height) || m_width <= 0 || m_height <= 0) {
			std::cerr << "Invalid texture dimensions: " << m_width << "x" << m_height << std::endl;
			return;
		}

		std::getline(file, line);
		int maxValue;
		std::istringstream maxValueStream(line);
		if (!(maxValueStream >> maxValue)) {
			std::cerr << "Failed to read maximum color value from file: " << m_path << std::endl;
			return;
		}

		m_data = new Vec4[m_width * m_height];
		for (int y = 0; y < m_height; ++y) {
			for (int x = 0; x < m_width; ++x) {
				int r, g, b;
				if (!(file >> r >> g >> b)) {
					std::cerr << "Failed to read pixel data at position (" << x << "," << y << ")" << std::endl;
					delete[] m_data;
					m_data = nullptr;
					return;
				}
				// 确保颜色值在有效范围内
				r = std::clamp(r, 0, maxValue);
				g = std::clamp(g, 0, maxValue);
				b = std::clamp(b, 0, maxValue);
				
				m_data[y * m_width + x] = Vec4(
					static_cast<float>(r) / maxValue,
					static_cast<float>(g) / maxValue,
					static_cast<float>(b) / maxValue,
					1.0f
				);
			}
		}
		file.close();
    return;
	}

	Vec4 Texture::Sample(Vec2 texCoords) const {
		if (!m_data) {
			std::cerr << "Texture sampling failed: texture data is null" << std::endl;
			return Vec4(1.0f, 0.0f, 1.0f, 1.0f); // 返回粉色以便调试
		}

		switch (m_filter) {
		case TextureFilter::NEAREST:
			return SampleNearest(texCoords);
		case TextureFilter::LINEAR:
			return SampleLinear(texCoords);
		default:
			return SampleNearest(texCoords);
		}
	}

	Vec4 Texture::SampleNearest(Vec2 texCoords) const {
		if (!m_data || m_width <= 0 || m_height <= 0) {
			return Vec4(1.0f, 0.0f, 1.0f, 1.0f); // 返回粉色表示错误
		}

		float u = (m_wrap == TextureWrap::REPEAT) ? RepeatCoord(texCoords.x) : ClampCoord(texCoords.x);
		float v = (m_wrap == TextureWrap::REPEAT) ? RepeatCoord(texCoords.y) : ClampCoord(texCoords.y);
		
		u = u * m_width;
		v = v * m_height;
		
		int x = static_cast<int>(std::floor(u));
		int y = static_cast<int>(std::floor(v));
		
		x = (x < 0) ? 0 : (x >= m_width ? m_width - 1 : x);
		y = (y < 0) ? 0 : (y >= m_height ? m_height - 1 : y);

		return m_data[y * m_width + x];
	}

	Vec4 Texture::SampleLinear(Vec2 texCoords) const {
		if (!m_data || m_width <= 1 || m_height <= 1) {
			return Vec4(1.0f, 0.0f, 1.0f, 1.0f); // 返回粉色表示错误
		}

		float u = (m_wrap == TextureWrap::REPEAT) ? RepeatCoord(texCoords.x) : ClampCoord(texCoords.x);
		float v = (m_wrap == TextureWrap::REPEAT) ? RepeatCoord(texCoords.y) : ClampCoord(texCoords.y);

		float fx = u * (m_width - 1);
		float fy = v * (m_height - 1);

		int x0 = static_cast<int>(std::floor(fx));
		int y0 = static_cast<int>(std::floor(fy));
		int x1 = std::min(x0 + 1, m_width - 1);
		int y1 = std::min(y0 + 1, m_height - 1);

		fx -= x0;
		fy -= y0;

		Vec4 c00 = m_data[y0 * m_width + x0];
		Vec4 c10 = m_data[y0 * m_width + x1];
		Vec4 c01 = m_data[y1 * m_width + x0];
		Vec4 c11 = m_data[y1 * m_width + x1];

		Vec4 a = c00 * (1.0f - fx) + c10 * fx;
		Vec4 b = c01 * (1.0f - fx) + c11 * fx;
		return a * (1.0f - fy) + b * fy;
	}

	float Texture::ClampCoord(float coord) const {
		return std::clamp(coord, 0.0f, 1.0f);
	}

	float Texture::RepeatCoord(float coord) const {
		coord = coord - std::floor(coord);
		return (coord < 0.0f) ? coord + 1.0f : coord;
	}

	void Texture::SetFilter(TextureFilter filter) {
		m_filter = filter;
	}

	void Texture::SetWrap(TextureWrap wrap) {
		m_wrap = wrap;
	}
}