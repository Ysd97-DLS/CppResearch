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
	enum class DepthFuncType {
		LESS,
		LEQUAL,
		ALWAYS,
	};
	template<typename vertex, typename uniforms, typename varyings>
	struct Program {
		bool EnableDepthTest = true;
		bool EnableWriteDepth = true;
		bool EnableBlend = true;
		bool EnableDoubleSided = false;
		DepthFuncType DepthFunc = DepthFuncType::LESS;
		using vertex_shader = void (*)(varyings&, const vertex&, const uniforms&);
		vertex_shader VertexShader;
		using fragment_shader = Vec4(*)(bool& discard, const varyings&, const uniforms&);
		fragment_shader FragmentShader;
		Program(const vertex_shader vertexShader,const fragment_shader fragmentShader) :VertexShader(vertexShader),FragmentShader(fragmentShader){}
	};
	class Renderer {
	private:
		static constexpr int RGS_MAX_VARYINGS = 9;
		enum class Plane {
			POSITIVE_W,
			POSITIVE_X,
			NEGATIVE_X,
			POSITIVE_Y,
			NEGATIVE_Y,
			POSITIVE_Z,
			NEGATIVE_Z,
		};
		struct BoundingBox {
			int m_minX, m_maxX, m_minY, m_maxY;
		};
	public:
		static bool PassDepthTest(const float writeDepth, const float fDepth, const DepthFuncType depthFunc);
		static bool IsVisible(const Vec4& clipPos);
		static bool InsidePlane(const Vec4& clipPos, const Plane plane);
		static float GetIntersectRatio(const Vec4& prev, const Vec4& curr, const Plane plane);
		static BoundingBox GetBoundingBox(const Vec4(&fragCoords)[3], const int width, const int height);
		static void CalculateWeights(float(&screenweights)[3], float(&weights)[3], const Vec4(&fragCoords)[3], const Vec2& screenPoint);
		static bool InsideTriangle(float(&weights)[3]);
		static bool IsBackFacing(const Vec4& a, const Vec4& b, const Vec4& c);
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
		template<typename varyings>
		static void LerpVaryings(varyings& out, const varyings(&varying)[3], float(&weights)[3], const int width, const int height) {
			out.ClipPos = varying[0].ClipPos * weights[0] + varying[1].ClipPos * weights[1] + varying[2].ClipPos * weights[2];
			out.NdcPos = out.ClipPos / out.ClipPos.w;
			out.NdcPos.w = 1.0f / out.ClipPos.w;
			out.FragPos.x = (out.NdcPos.x + 1.0f) * 0.5f * width;
			out.FragPos.y = (out.NdcPos.y+ 1.0f) * 0.5f * height;
			out.FragPos.z = (out.NdcPos.z + 1.0f) * 0.5f;
			out.FragPos.w = out.NdcPos.w;
			constexpr int floatOffset = sizeof(Vec4) * 3 / sizeof(float);
			constexpr int floatNum = sizeof(varyings) / sizeof(float);
			float* v0 = (float*)&varying[0];
			float* v1 = (float*)&varying[1];
			float* v2 = (float*)&varying[2];
			float* floatOut = (float*)&out;
			for (int i = floatOffset; i < (int)floatNum;i++) {
				floatOut[i] = v0[i] * weights[0] + v1[i] * weights[1] + v2[i] * weights[2];
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

			CalculateNdcPos(varying, vertexNum);
			int fWidth = framebuffer.GetWidth();
			int fHeight = framebuffer.GetHeight();
			CalculateFragPos(varying, vertexNum, (float)fWidth, (float)fHeight);
			for (int i = 0; i < vertexNum - 2; i++) {
				varyings triVaryings[3];
				triVaryings[0] = varying[0];
				triVaryings[1] = varying[i + 1];
				triVaryings[2] = varying[i + 2];
				RasterizeTriangle(framebuffer, program, triVaryings, uniform);
			}
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
				const varyings& currVaryings = in[currIdx];
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
			}
			return outNum;
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
		template<typename varyings>
		static void CalculateNdcPos(varyings(&varying)[RGS_MAX_VARYINGS], const int vertexNum) {
			for (int i = 0;i < vertexNum;i++) {
				float w = varying[i].ClipPos.w;
				varying[i].NdcPos = varying[i].ClipPos / w;
				varying[i].NdcPos.w = 1.0f / w;
			}
		}
		template<typename varyings>
		static void CalculateFragPos(varyings(&varying)[RGS_MAX_VARYINGS], const int vertexNum, const float width, const float height) {
			for (int i = 0;i < vertexNum;i++) {
				float x = ((varying[i].NdcPos.x + 1.0f) * 0.5f * width);
				float y = ((varying[i].NdcPos.y + 1.0f) * 0.5f * height);
				float z = (varying[i].NdcPos.z + 1.0f) * 0.5f;
				float w = varying[i].NdcPos.w;
				varying[i].FragPos.x = x;
				varying[i].FragPos.y = y;
				varying[i].FragPos.z = z;
				varying[i].FragPos.w = w;
			}
		}
		template<typename vertex,typename uniforms,typename varyings>
		static void ProcessPixel(Framebuffer& framebuffer, const int x, const int y, const Program<vertex, uniforms, varyings>& program, const varyings& varying, const uniforms& uniform) {
			bool discard = false;
			Vec4 color{ 0.0f,0.0f,0.0f,0.0f };
			color = program.FragmentShader(discard, varying, uniform);
			if (discard) { return; }
			color.x = Clamp(color.x, 0.0f, 1.0f);
			color.y = Clamp(color.y, 0.0f, 1.0f);
			color.z = Clamp(color.z, 0.0f, 1.0f);
			color.w = Clamp(color.w, 0.0f, 1.0f);
			framebuffer.SetColor(x, y, color);
			if (program.EnableWriteDepth) {
				float depth = varying.FragPos.z;
				framebuffer.SetDepth(x, y, depth);
			}
		}
		template<typename vertex,typename uniforms, typename varyings>
		static void RasterizeTriangle(Framebuffer& framebuffer, const Program<vertex,uniforms,varyings>& program, const varyings(&varying)[3],const uniforms& uniform){
			if (!program.EnableDoubleSided) {
				bool isBackFacing = false;
				isBackFacing = IsBackFacing(varying[0].NdcPos, varying[1].NdcPos, varying[2].NdcPos);
				if (isBackFacing) {
					return;
				}
			}
			Vec4 fragCoords[3];
			fragCoords[0] = varying[0].FragPos;
			fragCoords[1] = varying[1].FragPos;
			fragCoords[2] = varying[2].FragPos;
			BoundingBox bBox = GetBoundingBox (fragCoords, framebuffer.GetWidth(), framebuffer.GetHeight());
			int width = framebuffer.GetWidth();
			int height = framebuffer.GetHeight();

			for (int y = bBox.m_minY; y <= bBox.m_maxY; y++) {
				for (int x = bBox.m_minX; x <= bBox.m_maxX; x++) {
					float screenWeights[3];
					float weights[3];
					Vec2 screenPoint{ (float)x + 0.5f,(float)y + 0.5f };
					CalculateWeights(screenWeights, weights, fragCoords, screenPoint);
					if (!InsideTriangle(weights))
						continue;
					varyings pixVaryings;
					LerpVaryings(pixVaryings, varying, weights, width, height);
					if (program.EnableDepthTest) {
						float depth = pixVaryings.FragPos.z;
						float fDepth = framebuffer.GetDepth(x, y);
						DepthFuncType depthFunc = program.DepthFunc;
						if (!PassDepthTest(depth, fDepth, depthFunc)) {
							continue;
						}
					}
					ProcessPixel(framebuffer, x, y, program, pixVaryings, uniform);
				}
			}
		}
	};
}