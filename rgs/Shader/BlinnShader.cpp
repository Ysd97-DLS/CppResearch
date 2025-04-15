#include "BlinnShader.h"

namespace RGS {
	void BlinnVertexShader(const BlinnVertex& vertex, const BlinnUniforms& uniforms, BlinnVaryings& varyings) {
		varyings.ClipPos = uniforms.m * vertex.ModelPos;
	}
}