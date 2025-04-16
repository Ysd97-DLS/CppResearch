#include "\cpp\CppResearch\rgs\Base\Maths.h"
#include "\cpp\CppResearch\rgs\Window\Framebuffer.h"
#include "\cpp\CppResearch\rgs\Shader\BaseShader.h"
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
		const vertex& operator[](size_t i) {
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
	public:
		static bool IsVisible(const Vec4& clipPos);
		template<typename vertex, typename uniforms, typename varyings>
		static void Draw(Framebuffer& framebuffer, const Program<vertex, uniforms, varyings>& program, const Triangle<vertex>& triangle, const uniforms& uniforms) {
			static_assert(std::is_base_of_v<BaseVertex, vertex>, "vertex must inherit from RGS::BaseVertex");
			static_assert(std::is_base_of_v<BaseVaryings, varyings>, "varyings must inherit from RGS::BaseVaryings");
			//创建顶点数组，三角形最多截出九边形
			varyings varying[RGS_MAX_VARYINGS] = { 0 };
			for (int i = 0; i != 3; ++i) {
				program.VertexShader(varying[i], triangle[i], uniforms);
			}
			int vertexNum = Clip(varying);
		}
		//返回顶点数目
		template<typename varyings>
		static int Clip(varyings(&varying)[RGS_MAX_VARYINGS]) {
			bool v0_visible = IsVisible(varying[0].ClipPos);
			bool v1_visible = IsVisible(varying[1].ClipPos);
			bool v2_visible = IsVisible(varying[2].ClipPos);
			if (v0_visible && v1_visible && v2_visible) {
				return 3;
			}
			int vertexNum = 3;

		}
	};
}