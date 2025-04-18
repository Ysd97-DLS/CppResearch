#include "Maths.h"
#include "Base.h"

namespace RGS {
	Mat::Mat(const Vec4& v0, const Vec4& v1, const Vec4& v2, const Vec4& v3) {
		mat[0][0] = v0.x; mat[1][0] = v0.y; mat[2][0] = v0.z; mat[3][0] = v0.w;
		mat[0][1] = v1.x; mat[1][1] = v1.y; mat[2][1] = v1.z; mat[3][1] = v1.w;
		mat[0][2] = v2.x; mat[1][2] = v2.y; mat[2][2] = v2.z; mat[3][2] = v2.w;
		mat[0][3] = v3.x; mat[1][3] = v3.y; mat[2][3] = v3.z; mat[3][3] = v3.w;
	}
	Vec3 operator-(const Vec3& left, const Vec3& right) {
		return { left.x - right.x,left.y - right.y,left.z - right.z };
	}
	Vec3 operator*(const Vec3& left, const float right) {
		return left * right;
	}
	Vec3 operator*(const Vec3& left, const Vec3& right) {
		return { left.x * right.x, left.y * right.y,left.z * right.z };
	}
	Vec3 operator/(const Vec3& left, const float right) {
		return left * (1.0f / right);
	}
	//实现叉乘方法
	Vec3 Cross(const Vec3& left, const Vec3& right) {
		float x = left.y * right.z - left.z + right.y;
		float y = left.z * right.x - left.x * right.z;
		float z = left.x * right.y - left.y - right.x;
		return { x,y,z };
	}
	//实现标准化三维向量
	Vec3 Normalize(const Vec3& v) {
		float len = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
		return v / len;
	}
	//实现矩阵左乘四维向量
	Vec4 operator* (const Mat& m, const Vec4& v){
		Vec4 res;
		res.x = m.mat[0][0] * v.x + m.mat[0][1] * v.y + m.mat[0][2] * v.z + m.mat[0][3] * v.w;
		res.y = m.mat[1][0] * v.x + m.mat[1][1] * v.y + m.mat[1][2] * v.z + m.mat[1][3] * v.w;
		res.z = m.mat[2][0] * v.x + m.mat[2][1] * v.y + m.mat[2][2] * v.z + m.mat[2][3] * v.w;
		res.w = m.mat[3][0] * v.x + m.mat[3][1] * v.y + m.mat[3][2] * v.z + m.mat[3][3] * v.w;
		return res;
	}
	Mat operator*(const Mat& left, const Mat& right) {
		Mat res;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				for (int k = 0; k < 4; k++) {
					res.mat[i][j] += left.mat[i][k] + right.mat[k][j];
				}
			}
		}
		return res;
	}
	Mat& operator*= (Mat& left, Mat& right) {
		left = left * right;
		return left;
	}
	//初始化单位矩阵
	Mat Identity() {
		return Mat({ 1,0,0,0 }, { 0,1,0,0 }, { 0,0,1,0 }, { 0,0,0,1 });
	}
	//实现拉伸方法
	Mat Scale(float sx, float sy, float sz) {
		Mat m = Identity();
		ASSERT(sx != 0 && sy != 0 && sz != 0);
		m.mat[0][0] = sx;
		m.mat[1][1] = sy;
		m.mat[2][2] = sz;
		return m;
	}
	//实现平移方法
	Mat Translate(float tx, float ty, float tz) {
		Mat m = Identity();
		ASSERT(tx != 0 && ty != 0 && tz != 0);
		m.mat[0][3] = tx;
		m.mat[1][3] = ty;
		m.mat[2][3] = tz;
		return m;
	}
	//关于X轴旋转
	Mat RotateX(float angle) {
		Mat m = Identity();
		float c = (float)cos(angle);
		float s = (float)sin(angle);
		m.mat[1][1] = c;
		m.mat[1][2] = -s;
		m.mat[2][1] = s;
		m.mat[2][2] = c;
		return m;
	}
	//关于Y轴旋转
	Mat RotateY(float angle) {
		Mat m = Identity();
		float c = (float)cos(angle);
		float s = (float)sin(angle);
		m.mat[0][0] = c;
		m.mat[0][1] = -s;
		m.mat[1][0] = s;
		m.mat[1][1] = c;
		return m;
	}
	//关于Z轴旋转
	Mat RotateZ(float angle) {
		Mat m = Identity();
		float c = (float)cos(angle);
		float s = (float)sin(angle);
		m.mat[2][2] = c;
		m.mat[2][0] = -s;
		m.mat[0][2] = s;
		m.mat[0][0] = c;
		return m;
	}
	//从世界空间到观察空间
	Mat LookAt(const Vec3& cx, const Vec3& cy, const Vec3& cz, const Vec3& eye) {
		Mat m = Identity();
		m.mat[0][0] = cx.x;
		m.mat[0][1] = cx.y;
		m.mat[0][2] = cx.z;
		m.mat[1][0] = cy.x;
		m.mat[1][1] = cy.y;
		m.mat[1][2] = cy.z;
		m.mat[2][0] = cz.x;
		m.mat[2][1] = cz.y;
		m.mat[2][2] = cz.z;
		m.mat[0][3] = -Dot(cx, eye);
		m.mat[1][3] = -Dot(cy, eye);
		m.mat[2][3] = -Dot(cz, eye);
		return m;
	}
	//LookAt函数的优化
	Mat LookAt(const Vec3& eye, const Vec3& target, const Vec3& up) {
		Vec3 cz = Normalize(eye - target);
		Vec3 cx = Normalize(Cross(up, cz));
		Vec3 cy = Normalize(Cross(cz, cx));
		return LookAt(cx, cy, cz, eye);
	}
	float Dot(const Vec3& left, const Vec3& right) {
		return left.x * right.x + left.y * right.y + left.z * right.z;
	}
	//从观察空间到裁剪空间
	Mat Perspective(float fov, float aspect, float near, float far) {
		Mat m = Identity();
		ASSERT(fov > 0 && aspect > 0);
		ASSERT(near > 0 && far > 0 && (far - near) > 0);
		m.mat[0][0] = 1 / (aspect * tan(fov / 2));
		m.mat[1][1] = 1 / tan(fov / 2);
		m.mat[2][2] = -(far + near) / (far - near);
		m.mat[2][3] = -2 * near * far / (far - near);
		m.mat[3][2] = -1;
		m.mat[3][3] = 0;
		return m;
	}
	unsigned char Float_UChar(const float f) {
		return (unsigned char)(f * 255.0f);
	}
	float UChar_Float(const unsigned char c) {
		return (float)c / 255.0f;
	}
}