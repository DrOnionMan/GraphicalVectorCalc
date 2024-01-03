#include"MKMath.h"
#include<math.h>
#include<iostream>
#include<windows.h>

namespace MKMaths {
	Mat4::Mat4() {

	}

	Mat4 Mat4::Rotate3D_z(f32 theta) {
		Mat4 Rot(
			{ cos(theta), -sin(theta), 0.0f, 0.0f },
			{ sin(theta),  cos(theta), 0.0f, 0.0f },
			{ 0.0f,     0.0f,       1.0f, 0.0f },
			{ 0.0f,     0.0f,       0.0f, 1.0f },
			false);
		return Rot;
	}



	Mat4 Mat4::Rotate3D_x(f32 theta) {
		Mat4 Rot(
			{ 1.0f,     0.0f,       0.0f,           0.0f },
			{ 0.0f,     1.0f,       0.0f,           0.0f },
			{ 0.0f,     0.0f,       cos(theta),  -sin(theta), },
			{ 0.0f,     0.0f,       sin(theta),   cos(theta), },
			false);
		return Rot;
	}



	Mat4 Mat4::Rotate3D_y(f32 theta) {
		Mat4 Rot(
			{ cos(theta),     0.0f,       sin(theta),   0.0f },
			{ 0.0f      ,     1.0f,       0.0f,         0.0f },
			{ -sin(theta),     0.0f,       cos(theta),   0.0f },
			{ 0.0f       ,     0.0f,       0.0f,         1.0f },
			false);
		return Rot;
	}

	Mat4 Mat4::Identity() {
		Mat4 Rot(
			{ 1.0f,     0.0f,       0.0f,   0.0f },
			{ 0.0f,     1.0f,       0.0f,   0.0f },
			{ 0.0f,     0.0f,       1.0f,   0.0f },
			{ 0.0f,     0.0f,       0.0f,   1.0f },
			false);
		return Rot;
	}



	Mat4 Mat4::strech(f32 sf_x, f32 sf_y, f32 sf_z) {
		Mat4 Rot(
			{ sf_x       ,     0.0f       ,       0.0f       ,   0.0f },
			{ 0.0f       ,     sf_y       ,       0.0f       ,   0.0f },
			{ 0.0f       ,     0.0f       ,       sf_z       ,   0.0f },
			{ 0.0f       ,     0.0f       ,       0.0f       ,   1.0f },
			false);
		return Rot;
	}

	Mat4 Mat4::Translate(f32 x, f32 y, f32 z) {
		Mat4 Rot(
			{ 1.0f,     0.0f,       0.0f,   x },
			{ 0.0f,     1.0f,       0.0f,   y },
			{ 0.0f,     0.0f,       1.0f,   z },
			{ 0.0f,     0.0f,       0.0f,   1.0f },
			false);
		return Rot;
	}

	//Angle is the argument from i
	Mat4 Mat4::Rotate2D(f32 theta) {
		Mat4 Rot(
			{ cos(theta)  ,   -sin(theta),  0.0f,  0.0f },
			{ sin(theta)  ,   cos(theta),  0.0f,  0.0f },
			{ 0.0f      ,     0.0f    ,  1.0f,  0.0f },
			{ 0.0f      ,     0.0f    ,  0.0f,  1.0f },
			false);

		return Rot;
	}





	Mat4::Mat4(vec4 v1, vec4 v2, vec4 v3, vec4 v4, bool columnwise) {

		if (columnwise) {
			mat[0][0] = v1.x;
			mat[1][0] = v1.y;
			mat[2][0] = v1.z;
			mat[3][0] = v1.a;

			mat[0][1] = v2.x;
			mat[1][1] = v2.y;
			mat[2][1] = v2.z;
			mat[3][1] = v2.a;

			mat[0][2] = v3.x;
			mat[1][2] = v3.y;
			mat[2][2] = v3.z;
			mat[3][2] = v3.a;

			mat[0][3] = v4.x;
			mat[1][3] = v4.y;
			mat[2][3] = v4.z;
			mat[3][3] = v4.a;
		}
		else {
			mat[0][0] = v1.x;
			mat[0][1] = v1.y;
			mat[0][2] = v1.z;
			mat[0][3] = v1.a;

			mat[1][0] = v2.x;
			mat[1][1] = v2.y;
			mat[1][2] = v2.z;
			mat[1][3] = v2.a;

			mat[2][0] = v3.x;
			mat[2][1] = v3.y;
			mat[2][2] = v3.z;
			mat[2][3] = v3.a;

			mat[3][0] = v4.x;
			mat[3][1] = v4.y;
			mat[3][2] = v4.z;
			mat[3][3] = v4.a;
		}
	}

	f32 Dot(vec4& left, vec4& right) {
		return (left.x * right.x) + (left.y * right.y) + (left.z * right.z);
	}

	// j = row of left matrix
	// k = index of column in right matrix
	// i = column of right matrix being multiplied

	Mat4 Mat4::Multiply(Mat4& left, Mat4& right) {

		vec4 matrix[4] = { 0 };
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				f32 sum = 0;
				for (int k = 0; k < 4; k++) {
					sum += left.mat[i][k] * right.mat[k][j];
				}
				switch (j) {
				case 0:matrix[i].x = sum; break;
				case 1:matrix[i].y = sum; break;
				case 2:matrix[i].z = sum; break;
				case 3:matrix[i].a = sum; break;
				default:
					break;
				}
			}
		}

		Mat4 retmat(matrix[0], matrix[1], matrix[2], matrix[3], false);
		return retmat;
	}

	std::ostream& operator<<(std::ostream& stream, vec4& vec) {
		stream << vec.x << " , " << vec.y << " , " << vec.z << " , " << vec.a;
		return stream;
	}

	void Mat4::getVecArrRowWise(vec4* vecs) {

		for (int i = 0; i < 4; i++) {
			vecs[i] = { this->mat[i][0],this->mat[i][1],this->mat[i][2],this->mat[i][3] };
		}
		return;
	}

	void Mat4::MultArr(Mat4& left, void** info) {
		vec4* VertexBuffer = (vec4*)info[0];
		UINT* start_offset = (UINT*)info[1];
		UINT* VertBuffSize = (UINT*)info[2];

		vec4 rows[4] = { 0 };
		left.getVecArrRowWise(rows);
		//for (int i = 0; i < 4; i++) {
		//	std::cout << rows[i] << std::endl;
		//}

		//std::cout << "Sawg" << std::endl;
		//std::cout << "\n\n";

		for (int i = 0; i < (*VertBuffSize - *start_offset); i++) {
			vec4* currentColumn = &VertexBuffer[i];
			vec4 Column = *currentColumn;
			//std::cout << "Current Column -> " << Column << std::endl;

			currentColumn->x = Dot(rows[0], Column);
			//std::cout << "Result of first dot -> " << currentColumn->x << std::endl;
			//std::cout << "Expected result -> " << cos(0.78) * Column.x + -sin(0.78) * Column.y << std::endl;
			currentColumn->y = Dot(rows[1], Column);
			currentColumn->z = Dot(rows[2], Column);
			currentColumn->a = Dot(rows[3], Column);


		}

		return;
	}



	void Mat4::transpose() {
		Mat4 matrix = *this;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				mat[i][j] = matrix.mat[j][i];
			}
		}
	}


	Mat4 operator*(Mat4 right, Mat4 left) {
		return Mat4::Multiply(right, left);
	}
}