#include "../Base/Base.h"
#include "Texture.h"
#include <stb_image/stb_image.h>
#include <iostream>

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
		int width, height, channels;
		
	}
}