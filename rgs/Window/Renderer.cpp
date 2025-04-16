#include "Renderer.h"

namespace RGS {
	bool Renderer::IsVisible(const Vec4& clipPos) {
		return fabs(clipPos.x) <= clipPos.w && fabs(clipPos.y) <= clipPos.w && fabs(clipPos.z) <= clipPos.w;
	}
}