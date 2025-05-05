#include "../Base/Base.h"
#include "Texture.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

namespace RGS {
	Texture::Texture(const std::string& path) :m_path(path) {
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
		std::getline(file, line);
		if (line != "P3") {
			std::cerr << "Unsupported PPM format. Only P3 (ASCII) is supported." << std::endl;
			return;
		}

		// 跳过注释行
		while (std::getline(file, line) && line[0] == '#');

		std::istringstream iss(line);
		if (!(iss >> m_width >> m_height)) {
			std::cerr << "Failed to read texture dimensions from file: " << m_path << std::endl;
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
					std::cerr << "Failed to read pixel data from file: " << m_path << std::endl;
					delete[] m_data;
					m_data = nullptr;
					return;
				}
				m_data[y * m_width + x] = Vec4(
					static_cast<float>(r) / maxValue,
					static_cast<float>(g) / maxValue,
					static_cast<float>(b) / maxValue,
					1.0f
				);
			}
		}

		file.close();
	}

	Vec4 Texture::Sample(Vec2 texCoords) const {
		if (!m_data) return Vec4(0.0f, 0.0f, 0.0f, 1.0f);

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
		float u = (m_wrap == TextureWrap::REPEAT) ? RepeatCoord(texCoords.x) : ClampCoord(texCoords.x);
		float v = (m_wrap == TextureWrap::REPEAT) ? RepeatCoord(texCoords.y) : ClampCoord(texCoords.y);

		int x = static_cast<int>(u * m_width);
		int y = static_cast<int>(v * m_height);

		x = std::clamp(x, 0, m_width - 1);
		y = std::clamp(y, 0, m_height - 1);

		return m_data[y * m_width + x];
	}

	Vec4 Texture::SampleLinear(Vec2 texCoords) const {
		float u = (m_wrap == TextureWrap::REPEAT) ? RepeatCoord(texCoords.x) : ClampCoord(texCoords.x);
		float v = (m_wrap == TextureWrap::REPEAT) ? RepeatCoord(texCoords.y) : ClampCoord(texCoords.y);

		u = u * (m_width - 1);
		v = v * (m_height - 1);

		int x0 = static_cast<int>(u);
		int y0 = static_cast<int>(v);
		int x1 = std::min(x0 + 1, m_width - 1);
		int y1 = std::min(y0 + 1, m_height - 1);

		float fx = u - x0;
		float fy = v - y0;

		Vec4 c00 = m_data[y0 * m_width + x0];
		Vec4 c01 = m_data[y0 * m_width + x1];
		Vec4 c10 = m_data[y1 * m_width + x0];
		Vec4 c11 = m_data[y1 * m_width + x1];

		Vec4 row0 = c00 * (1 - fx) + c01 * fx;
		Vec4 row1 = c10 * (1 - fx) + c11 * fx;

		return row0 * (1 - fy) + row1 * fy;
	}

	float Texture::ClampCoord(float coord) const {
		return std::clamp(coord, 0.0f, 1.0f);
	}

	float Texture::RepeatCoord(float coord) const {
		coord = coord - std::floor(coord);
		if (coord < 0) coord += 1.0f;
		return coord;
	}

	void Texture::SetFilter(TextureFilter filter) {
		m_filter = filter;
	}

	void Texture::SetWrap(TextureWrap wrap) {
		m_wrap = wrap;
	}
}