#pragma once
typedef float f32;
typedef int i32;


enum GeomType {
	NULLTYPE = -1, CIRCLE = 1,CNUM,  HLINE
};

class GeomData {
public:
	bool operator!() {
		return this->gType == NULLTYPE ? true : false;
	}
	union {
		struct {
			f32 xspos;
			f32 yspos;
			f32 arg;
		}hline;
		struct {
			f32 center_y;
			f32 center_x;
			f32 radius;
		}circle;
		struct {
			f32 mag;
			f32 arg;
		}comp_num;
	}data;
	char name;
	GeomType gType;
};




