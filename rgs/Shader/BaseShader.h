#pragma once
#include "../Base/Maths.h"
#include <string>

namespace RGS {
	struct BaseVertex {
		Vec2 TexCoord;
		Vec4 ModelPos = { 0,0,0,1 };
		operator const std::string() const {
			return "ModelPos: " + (std::string)ModelPos;
		}
	};
	struct BaseVaryings {
		Vec4 ClipPos = { 0.0f,0.0f,0.0f,1.0f };
		Vec4 NdcPos = { 0.0f,0.0f,0.0f,1.0f };
		Vec4 FragPos = { 0.0f,0.0f,0.0f,1.0f };
		Vec2 TexCoord;
	};
	struct BaseUniforms {
		Mat mvp;
		operator const std::string() const {
			return (std::string)mvp;
		}
	};
}