#pragma once
#include"Complex.h"
#include<math.h>
#include<iostream>
#include<windows.h>

typedef float f32;
typedef uint8_t ui8;
namespace MKMaths {
	

	struct vec4 {
		f32 x;
		f32 y;
		f32 z;
		f32 a;
	};

	template<typename T>
	T Abs(T num) {
		return num > 0 ? num : (num * -1);
	}

	struct color {
		ui8 r, g, b, a;
	};

	struct vertex {
		MKMaths::vec4 pos;
		color col;
	};

	f32 Dot(vec4& left, vec4& right);

	class Mat4 {
	public:

		Mat4();
		Mat4(vec4 v1, vec4 v2, vec4 v3, vec4 v4, bool columnwise);


		void transpose();
		void operator*(void** vertexBufferData) {
			MultArr(*this, vertexBufferData);
		}
		void operator~() {
			this->transpose();
		}

		Mat4 operator*(Mat4& other) {
			return Multiply(*this, other);
		}





		static Mat4 Rotate3D_z(f32 theta);
		static Mat4 Rotate3D_x(f32 theta);
		static Mat4 Rotate3D_y(f32 theta);
		static Mat4 Identity();
		static Mat4 Rotate2D(f32 theta);
		static Mat4 Multiply(Mat4& left, Mat4& right);
		static Mat4 strech(f32 sf_x, f32 sf_y, f32 sf_z);
		static Mat4 Translate(f32 x, f32 y, f32 z);
		/*
			Info contains:
			Info[0] -> VertexBuffer
			Info[1] -> start offset
			Info[2] -> length of the vertexBuffer
		*/
		void MultArr(Mat4& left, void** info);


		void getVecArrRowWise(vec4* vecs);
		
		f32 mat[4][4];
	};

	Mat4 operator*(Mat4 right, Mat4 left);
}