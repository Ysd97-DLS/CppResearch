#include "BlinnShader.h"

namespace RGS {
	void BlinnVertexShader(BlinnVaryings& varyings, const BlinnVertex& vertex, const BlinnUniforms& uniforms) {
		varyings.ClipPos = uniforms.mvp * vertex.ModelPos;
		varyings.TexCoord = vertex.TexCoord;
	}
	Vec4 BlinnFragmentShader(bool& discard, const BlinnVaryings& varyings, const BlinnUniforms& uniforms) {
		return Vec4{ varyings.TexCoord.x, varyings.TexCoord.y,0.0f,1.0f };
	}
}