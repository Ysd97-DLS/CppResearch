#include "BlinnShader.h"

namespace RGS {
	void BlinnVertexShader(BlinnVaryings& varyings, const BlinnVertex& vertex, const BlinnUniforms& uniforms) {
		varyings.ClipPos = uniforms.mvp * vertex.ModelPos;
	}
	Vec4 BlinnFragmentShader(bool& discard, const BlinnVaryings& varyings, const BlinnUniforms& uniforms) {
		discard = false;
		return{ varyings.NdcPos.x / 2 + 0.5f,varyings.NdcPos.y / 2 + 0.5f,0.0f,1.0f };
	}
}