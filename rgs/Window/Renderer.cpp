#include "Renderer.h"

namespace RGS {
	bool Renderer::IsVisible(const Vec4& clipPos) {
		return fabs(clipPos.x) <= clipPos.w && fabs(clipPos.y) <= clipPos.w && fabs(clipPos.z) <= clipPos.w;
	}
	bool Renderer::InsidePlane(const Vec4& clipPos, const Plane plane) {
		switch (plane) {
		case Plane::POSITIVE_W:
			return clipPos.w >= 0.0f;
		case Plane::POSITIVE_X:
			return clipPos.x <= clipPos.w;
		case Plane::NEGATIVE_X:
			return clipPos.x >= -clipPos.w;
		case Plane::POSITIVE_Y:
			return clipPos.y <= clipPos.w;
		case Plane::NEGATIVE_Y:
			return clipPos.y >= -clipPos.w;
		case Plane::POSITIVE_Z:
			return clipPos.z <= clipPos.w;
		case Plane::NEGATIVE_Z:
			return clipPos.z >= -clipPos.w;
		default:
			ASSERT(false);
			return false;
		}
	}
}