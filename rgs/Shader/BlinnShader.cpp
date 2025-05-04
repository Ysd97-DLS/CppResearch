#include "BlinnShader.h"

namespace RGS {
	void BlinnVertexShader(BlinnVaryings& varyings, const BlinnVertex& vertex, const BlinnUniforms& uniforms) {
		varyings.ClipPos = uniforms.mvp * vertex.ModelPos;
		varyings.TexCoord = vertex.TexCoord;
		varyings.WorldPos = uniforms.Model * vertex.ModelPos;
		varyings.WorldNormal = uniforms.ModelNormalToWorld * Vec4{ vertex.ModelNormal,0.0f };
	}
	Vec4 BlinnFragmentShader(bool& discard, const BlinnVaryings& varyings, const BlinnUniforms& uniforms) {
		discard = false;
		const Vec3& cameraPos = uniforms.CameraPos;
		const Vec3& lightPos = uniforms.LightPos;
		const Vec3& worldPos = varyings.WorldPos;
		Vec3 worldNormal = Normalize(varyings.WorldNormal);
		Vec3 viewDir = Normalize(cameraPos - worldPos);
		Vec3 lightDir = Normalize(lightPos - worldPos);
		Vec3 halfDir = Normalize(lightDir + viewDir);
		const Vec3& ambient = uniforms.LightAmbient;
		Vec3 diffuse = std::max(0.0f, Dot(worldNormal, lightDir)) * uniforms.LightDiffuse;
		Vec3 specular = (float)pow(std::max(0.0f, Dot(halfDir, worldNormal)), uniforms.Shininess) * uniforms.LightSpecular;
		Vec3 result = (ambient + diffuse + specular) * uniforms.ObjectColor;
		return { result, 1.0f };
	}
}