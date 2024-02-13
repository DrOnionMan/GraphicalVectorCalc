#include"MKMath.h"
#include<math.h>
#include<iostream>
#include<windows.h>
#include<omp.h>

namespace MKMaths {
	

	Mat4 Mat4::Rotate3D_z(f32 theta) {
		f32 Rot[16] = {
			 cos(theta), -sin(theta), 0.0f, 0.0f ,
			 sin(theta),  cos(theta), 0.0f, 0.0f ,
			 0.0f,     0.0f,       1.0f, 0.0f ,
			 0.0f,     0.0f,       0.0f, 1.0f ,
		};
		Mat4 ret(Rot);
		return ret;
	}



	Mat4 Mat4::Rotate3D_x(f32 theta) {
		f32 Rot[16] = {
			 1.0f,     0.0f,       0.0f,           0.0f ,
			 0.0f,     1.0f,       0.0f,           0.0f ,
			 0.0f,     0.0f,       cos(theta),  -sin(theta),
			 0.0f,     0.0f,       sin(theta),   cos(theta),
		};

		Mat4 ret(Rot);
			
		return ret;
	}



	Mat4 Mat4::Rotate3D_y(f32 theta) {
		f32 Rot[16] = {
			cos(theta), 0.0f, sin(theta), 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			-sin(theta), 0.0f, cos(theta), 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f,
		};
		Mat4 ret(Rot);
		return ret;
	}

	Mat4 Mat4::Identity() {
		f32 Id[16] = {
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f,
		};
		Mat4 ret(Id);
		return ret;
	}



	Mat4 Mat4::strech(f32 sf_x, f32 sf_y, f32 sf_z) {
		f32 strech[16] = {
			sf_x, 0.0f, 0.0f, 0.0f,
			0.0f, sf_y, 0.0f, 0.0f,
			0.0f, 0.0f, sf_z, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f,
		};
		Mat4 ret(strech);
		return ret;
	}

	Mat4 Mat4::Translate(f32 x, f32 y, f32 z) {
		f32 Rot[16] = {
			1.0f,     0.0f,       0.0f,   x ,
			0.0f,     1.0f,       0.0f,   y ,
			0.0f,     0.0f,       1.0f,   z ,
			0.0f,     0.0f,       0.0f,   1.0f ,
		};
		Mat4 ret(Rot);

		return ret;
	}

	//Angle is the argument from i
	Mat4 Mat4::Rotate2D(f32 theta) {
		f32 arr[16] = {
			cos(theta)  ,   -sin(theta),  0.0f,  0.0f ,
			sin(theta)  ,   cos(theta),  0.0f,  0.0f ,
			0.0f      ,     0.0f    ,  1.0f,  0.0f ,
			0.0f      ,     0.0f    ,  0.0f,  1.0f ,
		};

		Mat4 ret(arr);

		return ret;
	}





	

	f32 Dot_4D(const vec4& left,const vec4& right) {
		return (left.x * right.x) + (left.y * right.y) + (left.z * right.z) + (left.a * right.a);
	}

	// j = row of left matrix
	// k = index of column in right matrix
	// i = column of right matrix being multiplied



#define Pragma_OMP _Pragma("\"omp parallel for\"")
	


	//transopses 4x4 matricies
	void Mat4::transpose() const {

		f32 arr[16] = { 0 };
		for (int n = 0; n < 16; n++) {
			int i = n / 4;
			int j = n % 4;
			arr[n] = this->mat[4 * j + i];
		}

		//Super sketchy cast
		memcpy(const_cast<f32*>(this->mat), arr, 16 * sizeof(float));
	}



	Pragma_OMP
	Mat4 Mat4::Mul(const Mat4& left, const Mat4& right, const size_t matSize) {
		Mat4 ret;

		alignas(64) float f[16] = { 0 };

		
			for (int i = 0; i < matSize; i++) {
				for (int j = 0; j < matSize; j++) {
					int cIndex = i * matSize + j;
					for (int k = 0; k < matSize; k++) {
						int aIndex = i * matSize + k;
						int bIndex = k * matSize + j;
						f[cIndex] += left.mat[aIndex] * right.mat[bIndex];
					}
				}
			}
		return ret(f);
	}

	
	Mat4 Mat4::__transpose(Mat4 m) {
		f32 arr[16] = { 0 };

		Pragma_OMP
			for (int n = 0; n < 16; n++) {
				int i = n / 4;
				int j = n % 4;
				arr[n] = m.mat[4 * j + i];
			}

		memcpy(m.mat, arr, 16* sizeof(f32));

		return m;
	}

	Mat4 Mat4::Rotate3D_z_t(f32 theta) {
		f32 Rot[16] = {
			 cos(theta), sin(theta), 0.0f, 0.0f ,
			 -sin(theta),  cos(theta), 0.0f, 0.0f ,
			 0.0f,     0.0f,       1.0f, 0.0f ,
			 0.0f,     0.0f,       0.0f, 1.0f ,
		};
		Mat4 ret(Rot);
		return ret;
	}

	Mat4 Mat4::Rotate3D_x_t(f32 theta) {
		f32 Rot[16] = {
			1,          0,           0, 0,
		0, cos(theta), -sin(theta), 0,
		0, sin(theta),  cos(theta), 0,
		0,          0,           0, 1
		};

		Mat4 ret(Rot);

		return ret;
	}
	Mat4 Mat4::Rotate3D_y_t(f32 theta) {
		f32 Rot[16] = {
		cos(theta), 0, sin(theta), 0,
		0, 1, 0, 0,
		-sin(theta), 0, cos(theta), 0,
		0, 0, 0, 1
		};
		Mat4 ret(Rot);
		return ret;
	}

	Mat4 Mat4::Rotate2D_t(f32 theta) {
		f32 arr[16] = {
			cos(theta)  ,   sin(theta),  0.0f,  0.0f ,
			-sin(theta)  ,   cos(theta),  0.0f,  0.0f ,
			0.0f      ,     0.0f    ,  1.0f,  0.0f ,
			0.0f      ,     0.0f    ,  0.0f,  1.0f ,
		};

		Mat4 ret(arr);

		return ret;
	}
	
	Mat4 Mat4::Translate_t(f32 x, f32 y, f32 z) {
		f32 Rot[16] = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		x, y, z, 1
		};
		Mat4 ret(Rot);

		return ret;
	}

	//Love the Formatting
	Mat4 Mat4::PerspectiveRH(f32 VW, f32 VH, f32 n, f32 f) {
		f32 r[16] = {
		(2 * n) / VW,    0,            0,                       0,
		0    ,          (2 * n) / VH,     0,                       0,
		0 ,                   0,      f / (n - f),              -1,
		0  ,                  0, f * n / (n - f), 0
		};
		Mat4 ret(r);

		return ret;
	}


}


MKMaths::Mat4 operator*(MKMaths::Mat4 left, MKMaths::Mat4 right) {
	return MKMaths::Mat4::Mul(left, right, 4);
}

#undef Pragma_OMP