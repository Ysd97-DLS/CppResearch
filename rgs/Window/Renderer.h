#include "\cpp\CppResearch\rgs\Base\Maths.h"
#include "\cpp\CppResearch\rgs\Window\Framebuffer.h"
#include "\cpp\CppResearch\rgs\Shader\BaseShader.h"
#include "\cpp\CppResearch\rgs\Base\Base.h"
#include <type_traits>
#include <cmath>

namespace RGS {
	template<typename vertex>
	struct Triangle {
		static_assert(std::is_base_of_v<BaseVertex, vertex>, "vertex must inherit from RGS::BaseVertex");
		vertex Vertex[3];
		vertex& operator[](size_t i) {
			return Vertex[i];
		}
		const vertex& operator[](size_t i)const {
			return Vertex[i];
		}
		Triangle() = default;
	};
	template<typename vertex, typename uniforms, typename varyings>
	struct Program {
		using vertex_shader = void (*)(varyings&, const vertex&, const uniforms&);
		vertex_shader VertexShader;
		Program(const vertex_shader vertexShader) :VertexShader(vertexShader) {

		}
	};
	class Renderer {
	private:
		static constexpr int RGS_MAX_VARYINGS = 9;
		enum class Plane{
			POSITIVE_W,
			POSITIVE_X,
			NEGATIVE_X,
			POSITIVE_Y,
			NEGATIVE_Y,
			POSITIVE_Z,
			NEGATIVE_Z,
		};
	public:
		static bool IsVisible(const Vec4& clipPos);
		static bool InsidePlane(const Vec4& clipPos, const Plane plane);
		static float GetIntersectRatio(const Vec4& prev, const Vec4& curr, const Plane plane);
		template<typename varyings>
		static void LerpVaryings(varyings& out, const varyings& start, const varyings& end, const float ratio) {
			constexpr int floatNum = sizeof(varyings) / sizeof(float);
			float* floatStart = (float*)&start;
			float* floatEnd = (float*)&end;
			float* floatOut = (float*)&out;
			for (int i = 0; i < (int)floatNum; i++) {
				floatOut[i] = Lerp(floatStart[i], floatEnd[i], ratio);
			}
		}
		template<typename vertex, typename uniforms, typename varyings>
		static void Draw(Framebuffer& framebuffer, const Program<vertex, uniforms, varyings>& program, const Triangle<vertex>& triangle, const uniforms& uniform) {
			static_assert(std::is_base_of_v<BaseVertex, vertex>, "vertex must inherit from RGS::BaseVertex");
			static_assert(std::is_base_of_v<BaseVaryings, varyings>, "varyings must inherit from RGS::BaseVaryings");
			//创建顶点数组，三角形最多截出九边形
			varyings varying[RGS_MAX_VARYINGS];
			for (int i = 0; i != 3; ++i) {
				program.VertexShader(varying[i], triangle[i], uniform);
			}
			int vertexNum = Clip(varying);
		}
		//返回顶点数目
		template<typename varyings>
		static int ClipAgainstPlane(varyings(&out)[RGS_MAX_VARYINGS], const varyings(&in)[RGS_MAX_VARYINGS], const Plane plane, const int inNum) {
			ASSERT(inNum >= 3);
			int outNum = 0;
			for (int i = 0; i < inNum; i++) {
				int prevIdx = (inNum - 1 + i) % inNum;
				int currIdx = i;
				const varyings& prevVaryings = in[prevIdx];
				const varyings& currVaryings = out[currIdx];
				const bool prevInside = InsidePlane(prevVaryings.ClipPos, plane);
				const bool currInside = InsidePlane(currVaryings.ClipPos, plane);
				if (prevInside != currInside) {
					float ratio = GetIntersectRatio(prevVaryings.ClipPos, currVaryings.ClipPos, plane);
					LerpVaryings(out[outNum], prevVaryings, currVaryings, ratio);
					outNum++;
				}
				if (currInside) {
					out[outNum] = in[currIdx];
					outNum++;
				}
				ASSERT(outNum <= RGS_MAX_VARYINGS);
				return outNum;
			}
		}
		template<typename varyings>
		static int Clip(varyings(&varying)[RGS_MAX_VARYINGS]) {
			bool v0_visible = IsVisible(varying[0].ClipPos);
			bool v1_visible = IsVisible(varying[1].ClipPos);
			bool v2_visible = IsVisible(varying[2].ClipPos); 
			if (v0_visible && v1_visible && v2_visible) {
				return 3;
			}
			int vertexNum = 3;
			varyings varying_out[RGS_MAX_VARYINGS];
			vertexNum = ClipAgainstPlane(varying_out, varying, Plane::POSITIVE_W, vertexNum);
			if (vertexNum == 0) {
				return 0;
			}
			vertexNum = ClipAgainstPlane(varying, varying_out, Plane::POSITIVE_X, vertexNum);
			if (vertexNum == 0) {
				return 0;
			}
			vertexNum = ClipAgainstPlane(varying_out, varying, Plane::NEGATIVE_X, vertexNum);
			if (vertexNum == 0) {
				return 0;
			}
			vertexNum = ClipAgainstPlane(varying, varying_out, Plane::POSITIVE_Y, vertexNum);
			if (vertexNum == 0) {
				return 0;
			}
			vertexNum = ClipAgainstPlane(varying_out, varying, Plane::NEGATIVE_Y, vertexNum);
			if (vertexNum == 0) {
				return 0;
			}
			vertexNum = ClipAgainstPlane(varying, varying_out, Plane::POSITIVE_Z, vertexNum);
			if (vertexNum == 0) {
				return 0;
			}
			vertexNum = ClipAgainstPlane(varying_out, varying, Plane::NEGATIVE_Z, vertexNum);
			if (vertexNum == 0) {
				return 0;
			}
			memcpy(varying, varying_out, sizeof(varying_out));
			return vertexNum;
		}
	};
}