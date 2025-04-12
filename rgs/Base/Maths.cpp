#include "Maths.h"

namespace RGS {
	unsigned char Float_UChar(const float f) {
		return (unsigned char)(f * 255.0f);
	}
	float UChar_Float(const unsigned char c) {
		return (float)c / 255.0f;
	}
}