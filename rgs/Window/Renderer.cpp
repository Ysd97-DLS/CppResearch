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
	float Renderer::GetIntersectRatio(const Vec4& prev, const Vec4& curr, const Plane plane) {
		switch (plane) {
		case Plane::POSITIVE_W:
			return (prev.w - 0.0f) / (prev.w - curr.w);
		case Plane::POSITIVE_X:
			return (prev.w - prev.x) / ((prev.w - prev.x) - (curr.w - curr.x));
		case Plane::NEGATIVE_X:
			return (prev.w + prev.x) / ((prev.w + prev.x) - (curr.w + curr.x));
		case Plane::POSITIVE_Y:
			return (prev.w - prev.y) / ((prev.w - prev.y) - (curr.w - curr.y));
		case Plane::NEGATIVE_Y:
			return (prev.w + prev.y) / ((prev.w + prev.y) - (curr.w + curr.y));
		case Plane::POSITIVE_Z:
			return (prev.w - prev.z) / ((prev.w - prev.z) - (curr.w - curr.z));
		case Plane::NEGATIVE_Z:
			return (prev.w + prev.z) / ((prev.w + prev.z) - (curr.w + curr.z));
		default:
			ASSERT(false);
			return 0.0f;
		}
	}
}