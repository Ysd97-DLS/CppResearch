#pragma once
#include <vector>
#include "../Base/Maths.h"

namespace RGS {
	enum class TextureFilter {
		NEAREST,
		LINEAR
	};

	enum class TextureWrap {
		REPEAT,
		CLAMP_TO_EDGE
	};

	class Texture {
	public:
		Texture();
		Texture(const std::string& path);
		~Texture();
		Vec4 Sample(Vec2 texCoords) const;
		void SetFilter(TextureFilter filter);
		void SetWrap(TextureWrap wrap);
	private:
		void Init();
		Vec4 SampleNearest(Vec2 texCoords) const;
		Vec4 SampleLinear(Vec2 texCoords) const;
		float ClampCoord(float coord) const;
		float RepeatCoord(float coord) const;
		int m_width, m_height, m_channels;
		std::string m_path;
		Vec4* m_data;
		TextureFilter m_filter = TextureFilter::NEAREST;
		TextureWrap m_wrap = TextureWrap::REPEAT;
	};
}