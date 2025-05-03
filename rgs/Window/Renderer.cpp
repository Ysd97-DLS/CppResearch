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
			return clipPos.x <= +clipPos.w;
		case Plane::NEGATIVE_X:
			return clipPos.x >= -clipPos.w;
		case Plane::POSITIVE_Y:
			return clipPos.y <= +clipPos.w;
		case Plane::NEGATIVE_Y:
			return clipPos.y >= -clipPos.w;
		case Plane::POSITIVE_Z:
			return clipPos.z <= +clipPos.w;
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
	Renderer::BoundingBox Renderer::GetBoundingBox(const Vec4(&fragCoords)[3], const int width, const int height) {
		auto listX = { fragCoords[0].x, fragCoords[1].x, fragCoords[2].x };
		auto listY = { fragCoords[0].y, fragCoords[1].y, fragCoords[2].y };
		float minX = std::min<float>(listX);
		float maxX = std::max<float>(listX);
		float minY = std::min<float>(listY);
		float maxY = std::max<float>(listY);
		minX = Clamp(minX, 0.0f, (float)(width - 1));
		maxX = Clamp(maxX, 0.0f, (float)(width - 1));
		minY = Clamp(minY, 0.0f, (float)(height - 1));
		maxY = Clamp(maxY, 0.0f, (float)(height - 1));
		BoundingBox bBox;
		bBox.m_minX = std::floor(minX);
		bBox.m_minY = std::floor(minY);
		bBox.m_maxX = std::floor(maxX);
		bBox.m_maxY = std::floor(maxY);
		return bBox;
	}
	void Renderer::CalculateWeights(float(&screenWeights)[3], float(&weights)[3], const Vec4(&fragCoords)[3], const Vec2& screenPoint) {
		Vec2 ab = fragCoords[1] - fragCoords[0];
		Vec2 ac = fragCoords[2] - fragCoords[0];
		Vec2 ap = screenPoint - fragCoords[0];
		float factor = 1.0f / (ab.x * ac.y - ab.y * ac.x);
		float s = (ac.y * ap.x - ac.x * ap.y) * factor;
		float t = (ab.x * ap.y - ab.y * ap.x) * factor;
		screenWeights[0] = 1 - s - t;
		screenWeights[1] = s;
		screenWeights[2] = t;

		float w0 = fragCoords[0].w * screenWeights[0];
		float w1 = fragCoords[1].w * screenWeights[1];
		float w2 = fragCoords[2].w * screenWeights[2];
		float normalizer = 1.0f / (w0 + w1 + w2);
		weights[0] = w0 * normalizer;
		weights[1] = w1 * normalizer;
		weights[2] = w2 * normalizer;
	}
	bool Renderer::InsideTriangle(float(&weights)[3]) {
		return weights[0] >= -EPSILON && weights[1] >= -EPSILON && weights[2] >= -EPSILON;
	}
}