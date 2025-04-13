#pragma once
#include <string>
#include <iostream>

namespace RGS {
	struct Vec2 {
		float x, y;
		constexpr Vec2() :x(0.0f), y(0.0f) {

		}
		constexpr Vec2(float x, float y) : x(x), y(y) {

		}
		operator std::string() const {
			std::string res;
			res += "(";
			res += std::to_string(x);
			res += ", ";
			res += std::to_string(y);
			res += ")";
			return res;
		}
	};
	struct Vec3 {
		float x;
		float y;
		float z;
		constexpr Vec3() :x(0.0f), y(0.0f), z(0.0f) {

		}
		constexpr Vec3(float x, float y, float z) : x(x), y(y), z(z) {

		}
		operator Vec2() const {
			return { x,y };
		}
		operator std::string() const {
			std::string res;
			res += "(";
			res += std::to_string(x);
			res += ", ";
			res += std::to_string(y);
			res += ", ";
			res += std::to_string(z);
			res += ")";
			return res;
		}
	};
	struct Vec4 {
		float x;
		float y;
		float z;
		float w;
		constexpr Vec4() :x(0.0f), y(0.0f), z(0.0f), w(0.0f) {

		}
		constexpr Vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {

		}
		operator Vec2() const {
			return { x,y };
		}
		operator Vec3() const {
			return { x,y,z };
		}
		operator std::string() const {
			std::string res;
			res += "(";
			res += std::to_string(x);
			res += ", ";
			res += std::to_string(y);
			res += ", ";
			res += std::to_string(z);
			res += ", ";
			res += std::to_string(w);
			res += ")";
			return res;
		}
	};
	struct Mat {
		float mat[4][4];
		constexpr Mat() : mat{ {0.0f, 0.0f, 0.0f, 0.0f},
					  {0.0f, 0.0f, 0.0f, 0.0f},
					  {0.0f, 0.0f, 0.0f, 0.0f},
					  {0.0f, 0.0f, 0.0f, 0.0f} } {

		}
		Mat(const Vec4& v0, const Vec4& v1, const Vec4& v2, const Vec4& v3);
		operator const std::string() const {
			std::string res;
			res += "(";
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					res += std::to_string(mat[i][j]);
					res += (i == 3 && j == 3) ? " )" : ", ";
				}
			}
		}
	};
	Vec4 operator* (const Mat& m, const Vec4& v);
	Mat operator*(const Mat& left, const Mat& right);
	Mat& operator*= (Mat& left, Mat& right);
	Mat Identity();
	Mat Scale(float sx, float sy, float sz);
	Mat Translate(float tx, float ty, float tz);
	Mat RotateX(float angle);
	Mat RotateY(float angle);
	Mat RotateZ(float angle);
	Mat LookAt(const Vec3& cx, const Vec3& cy, const Vec3& cz, const Vec3& eye);
	float Dot(const Vec3& left, const Vec3& right);
	unsigned char Float_UChar(const float f);
	float UChar_Float(const unsigned char c);
}