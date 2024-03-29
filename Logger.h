#pragma once
#include<iostream>
#include<sstream>
#include<fstream>
#include"LinkedList.h"
#include"MKMath.h"
#include<filesystem>

//not included in release build
#ifndef NDEBUG
class Log {
public:
	Log() {
		
		File.open(f, std::ios::in | std::ios::out);
		std::string line;
		while (getline(File, line)) {
			string << line << "\n";
		}
		File.close();
		
	}

	~Log() {
		
		
		if (File.is_open()) {
			File.close();
		}
		
	}

	void operator<<(std::ostringstream& oss) {
		string << oss.str();
	}

	void operator<<(const char*  s) {
		std::string ss = s;
		string << ss;
	}

	void operator<<(MKMaths::Mat4& matrix) {
		string << "Matrix: \n";
		for (int i = 0; i <= 12; i += 4) {
			string << matrix.mat[i] << " , " << matrix.mat[i + 1] << " , " << matrix.mat[i + 2] << " , " << matrix.mat[i + 3] << "\n";
		}

		string << "\n\n";
	}

	void WipeFile() {
		
		fopen_s(&file, f.c_str(), "w");
		//love me an assert
		//make sure file is not NULL
		assert(file != NULL);
		fprintf_s(file, "\n");
		fclose(file);
	}

	void operator!() {
		Commit();
	}

	void operator<<(MKMaths::vec4& vector) {
		string << "Vector:\n";
		string << vector.x << " , " << vector.y << " , " << vector.z << " , " << vector.a << "\n";
	}

	void operator<<(int x) {
		string << x << std::endl;
	}

	void operator<<(float x) {
		string << x << std::endl;
	}

	void operator<<(UINT x) {
		string << x << std::endl;
	}

	void operator<<(MKMaths::vertex x) {
		string << "position :\n"
			<< x.pos.x << " , " << x.pos.y << " , " << x.pos.z << " , " << x.pos.a << "\n"
			<< "Colour:\n" << " , " << (UINT)x.col.r << " , " << (UINT)x.col.g << " , " << (UINT)x.col.b << " , " << (UINT)x.col.a << std::endl;
	}

	void operator<<(node* list) {
		for (node* p = list; p != NULL; p = p->next) {
			switch (p->data.gType) {
			case CIRCLE: {
				string << "Geometry ID -> " << p->data.name << "\n";
				string << "Circle centre -> (" << p->data.data.circle.center_x << " , " << p->data.data.circle.center_y << ")" << "\n";
				string << "Circle radius -> " << p->data.data.circle.radius << "\n";
			}break;
			case HLINE: {
				string << "Geometry ID -> " << p->data.name << "\n";
				string << "Halfline start -> (" << p->data.data.hline.xspos << " , " << p->data.data.hline.yspos << ")\n";
				string << "Halfline argument -> " << p->data.data.hline.arg << "\n";
			}break;
			case CNUM: {
				string << "Geometry ID -> " << p->data.name << "\n";
				string << "Complex Number argument -> " << p->data.data.comp_num.arg << "\n";
				string << "Complex Number magnitude -> " << p->data.data.comp_num.mag << "\n";
			}break;
			default: {
				string << "Invalid Geometry passed to logger" << std::endl;
			}break;
			}
		}
	}

	void operator<<(MKMaths::plane p) {
		string << "Plane equasion : \n" << p.a << "x + " << p.b << "y + " << p.c << "z = " << p.d;
		string << "\n";
	}

	

	void Commit() {
		if (File.is_open()) {
			File << string.str();
		}
		else {
			File.open(f);
			File << string.str();
			File.close();
		}
		string.flush();
	}

private:
	std::string f = "Log.txt";
	std::fstream File;
	std::ostringstream string;
	FILE* file;
};
#endif