#include "Maths.h"
#include "Base.h"

namespace RGS {
	Mat::Mat(const Vec4& v0, const Vec4& v1, const Vec4& v2, const Vec4& v3) {
		mat[0][0] = v0.x; mat[1][0] = v0.y; mat[2][0] = v0.z; mat[3][0] = v0.w;
		mat[0][1] = v1.x; mat[1][1] = v1.y; mat[2][1] = v1.z; mat[3][1] = v1.w;
		mat[0][2] = v2.x; mat[1][2] = v2.y; mat[2][2] = v2.z; mat[3][2] = v2.w;
		mat[0][3] = v3.x; mat[1][3] = v3.y; mat[2][3] = v3.z; mat[3][3] = v3.w;
	}
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
	Mat Identity() {
		return Mat({ 1,0,0,0 }, { 0,1,0,0 }, { 0,0,1,0 }, { 0,0,0,1 });
	}
	Mat Scale(float sx, float sy, float sz) {
		Mat m = Identity();
		ASSERT(sx != 0 && sy != 0 && sz != 0);
		m.mat[0][0] = sx;
		m.mat[1][1] = sy;
		m.mat[2][2] = sz;
		return m;
	}
	Mat Translate(float tx, float ty, float tz) {
		Mat m = Identity();
		ASSERT(tx != 0 && ty != 0 && tz != 0);
		m.mat[0][3] = tx;
		m.mat[1][3] = ty;
		m.mat[2][3] = tz;
		return m;
	}
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
	float Dot(const Vec3& left, const Vec3& right) {
		return left.x * right.x + left.y * right.y + left.z * right.z;
	}
	unsigned char Float_UChar(const float f) {
		return (unsigned char)(f * 255.0f);
	}
	float UChar_Float(const unsigned char c) {
		return (float)c / 255.0f;
	}
}