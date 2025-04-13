#include "Maths.h"

namespace RGS {
	Mat::Mat(const Vec4& v0, const Vec4& v1, const Vec4& v2, const Vec4& v3) {
		mat[0][0] = v0.x; mat[1][0] = v0.y; mat[2][0] = v0.z; mat[3][0] = v0.w;
		mat[0][1] = v1.x; mat[1][1] = v1.y; mat[2][1] = v1.z; mat[3][1] = v1.w;
		mat[0][2] = v2.x; mat[1][2] = v2.y; mat[2][2] = v2.z; mat[3][2] = v2.w;
		mat[0][3] = v3.x; mat[1][3] = v3.y; mat[2][3] = v3.z; mat[3][3] = v3.w;
	}
	unsigned char Float_UChar(const float f) {
		return (unsigned char)(f * 255.0f);
	}
	float UChar_Float(const unsigned char c) {
		return (float)c / 255.0f;
	}
}