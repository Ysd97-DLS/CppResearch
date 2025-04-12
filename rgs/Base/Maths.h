#pragma once

namespace RGS {
	struct Vec3 {
		float x;
		float y;
		float z;
		constexpr Vec3() :x(0.0f), y(0.0f), z(0.0f) {

		}
		constexpr Vec3(float x, float y, float z) : x(x), y(y), z(z) {

		}
	};
	unsigned char Float_UChar(const float f);
	float UChar_Float(const unsigned char c);
}