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
	void BlinnVertexShader(const BlinnVertex& vertex, const BlinnUniforms& uniforms, BlinnVaryings& varyings);
}