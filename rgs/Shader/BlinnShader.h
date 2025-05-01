#pragma once
#include "BaseShader.h"
#include "\cpp\CppResearch\rgs\Base\Maths.h"

namespace RGS {
	struct BlinnVertex :public BaseVertex {

	};
	struct BlinnVaryings : public BaseVaryings {

	};
	struct BlinnUniforms :public BaseUniforms {

	};
	void BlinnVertexShader(BlinnVaryings& varyings, const BlinnVertex& vertex, const BlinnUniforms& uniforms);
	Vec4 BlinnFragmentShader(bool& discard, const BlinnVaryings& varyings, const BlinnUniforms& uniforms);
}