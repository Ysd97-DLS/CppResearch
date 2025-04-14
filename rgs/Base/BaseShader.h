#pragma once
#include "Maths.h"
#include <string>

namespace RGS {
	struct BaseVertex {
		Vec4 ModelPos = { 0,0,0,1 };
		operator const std::string() const {
			return "ModelPos: " + (std::string)ModelPos;
		}
	};
	struct BaseVaryings {
		Vec4 ClipPos = { 0,0,0,1 };
	};
	struct BaseUniforms {
		Mat m;
		operator const std::string() const {
			return (std::string)m;
		}
	};
}