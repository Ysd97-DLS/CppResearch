#pragma once
#include <vector>
#include "../Base/Maths.h"

namespace RGS {
	class Texture {
	public:
		Texture(const std::string& path);
		~Texture();
		Vec4 Sample(Vec2 texCoords) const;
	private:
		void Init();
		int m_width, m_height, m_channels;
		std::string m_path;
		Vec4* m_data;
	};
}