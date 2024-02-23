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

	struct plane {
		f32 a;
		f32 b;
		f32 c;
		f32 d;
	};

	template<typename T>
	T Abs(T num) {
		return num > 0 ? num : (num * -1);
	}

	struct color {
		ui8 r, g, b, a;
	};


	//order is important for graphics & shader pipeline
	struct vertex {
		MKMaths::vec4 pos;
		color col;
	};

	f32 Dot(vec4& left, vec4& right);

	struct polar {
		f32 len;
		f32 theta;
		f32 phi;
	};

	
	
	inline f32 Mag(const plane p);

	polar toPolar(plane p);


	

	class Mat4 {
	public:
#define MATSIZE 16 * sizeof(float)
		
		Mat4(f32* floatarray) {
			std::memcpy(this->mat, floatarray, MATSIZE);
		}
		//sets uninitialised matrix to 0;
		Mat4() {
			std::memset(this->mat, 0.0f, MATSIZE);
		}


		//operators
		void operator=(f32* floatarray) {
			std::memcpy(this->mat, floatarray, MATSIZE);
		}
		void operator=(const Mat4& matrix) {
			std::memcpy(this->mat, matrix.mat, MATSIZE);
		}
		void operator!() const {
			this->transpose();
		}
		
		void operator~() {
			std::memset(this->mat, 0.0f, MATSIZE);
		}

		Mat4 operator()(float* f) {
			std::memcpy(this->mat, f, MATSIZE);
			return *this;
		}

		//SA for shader array
		void ToShader(f32 SA[4][4]) const {
			for (int i = 0; i < 4; ++i) {
				for (int j = 0; j < 4; ++j) {
					SA[i][j] = this->mat[i * 4 + j];
				}
			}
		}



		void transpose() const;
		static Mat4 Rotate3D_z(f32 theta);
		static Mat4 Rotate3D_x(f32 theta);
		static Mat4 Rotate3D_y(f32 theta);
		static Mat4 Identity();
		static Mat4 Rotate2D(f32 theta);
		//added matdim to allow for mult with vectors if needed;
		
		static Mat4 strech(f32 sf_x, f32 sf_y, f32 sf_z);
		static Mat4 Translate(f32 x, f32 y, f32 z);

		static Mat4 Mul(const Mat4& left, const Mat4& right, const size_t MatDim);

		//Transposed Matricies because directx
		static Mat4 __transpose(Mat4 matrix);
		static Mat4 Rotate3D_z_t(f32 theta);
		static Mat4 Rotate3D_x_t(f32 theta);
		static Mat4 Rotate3D_y_t(f32 theta);
		static Mat4 Rotate2D_t(f32 theta);
		static Mat4 Translate_t(f32 x, f32 y, f32 z);
		static Mat4 PerspectiveRH(f32 VW, f32 VH, f32 n, f32 f);

		/*
			Info contains:
			Info[0] -> VertexBuffer
			Info[1] -> start offset
			Info[2] -> length of the vertexBuffer
		*/

		
		


		
		alignas(64) f32 mat[16];


	};
	
	



#undef MATSIZE
}

MKMaths::Mat4 operator*(MKMaths::Mat4 left, MKMaths::Mat4 right);