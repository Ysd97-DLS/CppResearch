#include "\cpp\CppResearch\rgs\Base\Maths.h"
#include "\cpp\CppResearch\rgs\Window\Framebuffer.h"
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
	template<typename vertex, typename uniforms, typename varings>
	struct Program {
		using vertex_shader = void (*)(varings&, const vertex&, const uniforms&);
		vertex_shader VertexShader;
		Program(const vertex_shader vertexShader) :VertexShader(vertexShader) {

		}
	};
	class Renderer {
	private:

	public:
		template<typename vertex, typename uniforms, typename varings>
		static void Draw(Framebuffer& framebuffer, const Program<vertex, uniforms, varings>& program, const Triangle<vertex>& triangle, const uniforms& uniforms) {

		}
	};
}