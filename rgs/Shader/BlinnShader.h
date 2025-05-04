#pragma once
#include "BaseShader.h"
#include "\cpp\CppResearch\rgs\Base\Maths.h"

namespace RGS {
	struct BlinnVertex :public BaseVertex {
		Vec3 ModelNormal;
		Vec2 TexCoord = { 0.0f,0.0f };
	};
	struct BlinnVaryings : public BaseVaryings {
		Vec2 TexCoord;
	};
	struct BlinnUniforms :public BaseUniforms {
		bool IsAnother = false;
	};
	void BlinnVertexShader(BlinnVaryings& varyings, const BlinnVertex& vertex, const BlinnUniforms& uniforms);
	Vec4 BlinnFragmentShader(bool& discard, const BlinnVaryings& varyings, const BlinnUniforms& uniforms);
}