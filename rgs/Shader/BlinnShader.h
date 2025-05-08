#pragma once
#include "BaseShader.h"
#include "../Base/Maths.h"
#include "../Texture/Texture.h"

namespace RGS {
	struct BlinnVertex :public BaseVertex {
		Vec3 ModelNormal;
		Vec2 TexCoord = { 0.0f,0.0f };
	};
	struct BlinnVaryings : public BaseVaryings {
		Vec2 TexCoord;
		Vec3 WorldPos;
		Vec3 WorldNormal;
	};
	struct BlinnUniforms :public BaseUniforms {
		BlinnUniforms(const std::string& path) : m_texture(path) {}
		Mat Model;
		Mat ModelNormalToWorld;
		Vec3 LightPos{ 0.0f,1.0f,2.0f };
		Vec3 LightAmbient{ 0.3f,0.3f,0.3f };
		Vec3 LightDiffuse{ 0.5f,0.5f,0.5f };
		Vec3 LightSpecular{ 1.0f,1.0f,1.0f };
		Vec3 ObjectColor{ 1.0f,1.0f,1.0f };
		Vec3 CameraPos;
		float Shininess = 64.0f;
		bool IsAnother = false;
		Texture m_texture;
	};
	void BlinnVertexShader(BlinnVaryings& varyings, const BlinnVertex& vertex, const BlinnUniforms& uniforms);
	Vec4 BlinnFragmentShader(bool& discard, const BlinnVaryings& varyings, const BlinnUniforms& uniforms);
}