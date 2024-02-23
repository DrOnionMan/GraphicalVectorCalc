#include"Scene2d.h"
#include"Logger.h"
#include<d3dcompiler.h>
#include<string>
#include<math.h>
#include<DirectXMath.h>
#include <d3d11.h>
#include <vector>
#include"StringUtils.h"
#include<cmath>
#include"LinkedList.h"


#pragma comment(lib,"d3d11.lib")
#pragma comment(lib, "d3dCompiler.lib")
#include<math.h>
#include<fenv.h>

typedef D3D11_PRIMITIVE_TOPOLOGY dpt;
typedef unsigned char uchar;
namespace dx = DirectX;
namespace wrl = Microsoft::WRL;

#define PAIR std::pair<f32, f32>



static void genIndexBufferTriangle(UINT* ind, UINT vertexCount, UINT& indexCount) {


	const UINT first = indexCount;
#ifndef NDEBUG
	assert(ind != NULL);
#endif
	ind[indexCount] = first;
	ind[++indexCount] = first + 2;
	ind[++indexCount] = first + 1;
	
	for (UINT i = first + 3, j = first + 2; i < vertexCount; i++, j++) {
		ind[++indexCount] = first;
		
		ind[++indexCount] = i;
		
		ind[++indexCount] = j;
	}
	ind[++indexCount] = vertexCount - 1;
	ind[++indexCount] = first;
	ind[++indexCount] = first + 1;
	indexCount++;
}


class flt {
public:
	float step;
	double x;
};
void operator++(flt& i) {
	i.x += i.step;
}

bool operator<(flt& i, double x) {
	return i.x < x ? true : false;
}


#define M_PI 3.14159265358
#define CP 3603
#define RAD(x) ((float)((x * M_PI) / 180.0f))

void Scene2d::genVertexBufferCircle(MKMaths::vertex* buffer, MKMaths::color& col, f32 radius, int steps) {
	using namespace MKMaths;
	buffer[0] = { 0.0f, 0.0f, 0.0f, 1.0f, col };
	double period = 360.0;
	static int count;
	int j;
	flt i;

	for (count = 0, j = 1, i = { 360.0f / (float)steps, 0 }; j < steps; j++, ++i, count++) {
		f32 s = cos(RAD(i.x));
		f32 p = sin(RAD(i.x));

		buffer[j] = { (meters.x * radius * cos(RAD(i.x))) ,   (meters.y * radius * sin(RAD(i.x))) , 0.0f, 1.0f, col };
	}
#ifndef NDEBUG 
	static bool w = false;
	if (w == false) {
		Log l;
		l << "Contents of Buffer :\n";
		for (int i = 0; i < 360; i++) {
			l << buffer[i];
			!l;
		}
		w = true;
	}
#endif

}

static bool lessthan(node* left, node* right) {
	if (left->data.gType < right->data.gType) {
		return true;
	}

	return false;
}



//z=1.414*e^1.45i
//|c-(1+2i)|=5
//Arg(c-(1+5i))=1.45
Scene2d::Scene2d(f32* swidth, f32* sheight, node* list, Graphics* __restrict gfx) : list(list), gfx(gfx), sh(sheight), sw(swidth), Dim(gfx->_Dim), 
	meters({}) {
	quicksort_c(this->list, last_node(this->list), lessthan);
	if (!gfx->_Dim) {
		GetMeterSize();
		DefineTheMeter();
	}
	//DrawAxis();
}

void Scene2d::Render() noexcept {
	GetMeterSize();
	DefineTheMeter();
	DrawAxis();

	int i = 4;
	for (node* currentnode = list; currentnode != NULL; currentnode = currentnode->next) {
		if (currentnode->data.gType == CIRCLE) {
			DrawCircle(currentnode->data, colours.at(i));
			colours.pop_back();
			i--;
		}
		if (currentnode->data.gType == HLINE) {
			DrawHline(currentnode->data, colours.at(i));
			colours.pop_back();
			i--;
		}
		if (currentnode->data.gType == CNUM) {
			DrawCNum(currentnode->data, colours.at(i));
			colours.pop_back();
			i--;
		}
	}
}

void Scene2d::DrawCNum(GeomData& g, MKMaths::color& c) {
	using namespace MKMaths;
	vertex linePart[2] = {
		{0.0f, 0.0f, 0.0f, 1.0f, c},
		{g.data.comp_num.mag, 0.0f, 0.0f, 1.0f, c},
	};

	vertex trianglePart[3] = {
		{-0.5f * meters.x, -0.5f * meters.y, 0.0f, 1.0f, c},
		{0.0f, 0.0f, 0.0f, 1.0f, c},
		{-0.5f * meters.x, 0.5f * meters.y, 0.0f, 1.0f, c},
	};



	Mat4 mat = Mat4::Rotate2D_t(g.data.comp_num.arg) * Mat4::strech(meters.x, meters.y, 1.0f);

	//lib - Line index Buffer
	UINT lib[2] = { 0, 1 };


	gfx->BindConstantBuffer_Matrix(mat);
	gfx->BindVertexBuffer(linePart, 2);
	gfx->BindIndexBuffer(lib, 2);


	gfx->Draw(2, dpt::D3D11_PRIMITIVE_TOPOLOGY_LINELIST, { L"VS_WTrans.cso", L"GeneralPS.cso" });


	~mat;


	mat =
		Mat4::Translate_t(g.data.comp_num.mag, 0.0f, 0.0f)*
		Mat4::Rotate2D_t(g.data.comp_num.arg)*
		Mat4::strech(meters.x, meters.y, 1.0f)
		;



	//Triangle index buffer
	UINT tib[3] = {
		0, 2, 1
	};


	gfx->BindConstantBuffer_Matrix(mat);
	gfx->BindVertexBuffer(trianglePart, 3);
	gfx->BindIndexBuffer(tib, 3);


	gfx->Draw(3, dpt::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, { L"VS_WTrans.cso", L"GeneralPS.cso" });


}


void Scene2d::DrawHline(GeomData& g, MKMaths::color& c) {
	using namespace MKMaths;
	vertex verts[2] = {
		{0.0f, 0.0f, 0.0f, 1.0f, c },
		{1.0f, 0.0f, 0.0f, 1.0f, c}
	};


	

	Mat4 mat =
		Mat4::Rotate2D_t(g.data.hline.arg) *
		Mat4::Translate_t(g.data.hline.xspos * meters.x, g.data.hline.yspos * meters.y, 0.0f);
		Mat4::strech(meters.aRx, meters.aRy, 1.0f);

	UINT Indicies[2] = {
		0, 1
	};


	gfx->BindConstantBuffer_Matrix(mat);
	gfx->BindVertexBuffer(verts, 2);
	gfx->BindIndexBuffer(Indicies, 2);


	gfx->Draw(2, dpt::D3D11_PRIMITIVE_TOPOLOGY_LINELIST, {  L"VS_WTrans.cso", L"GeneralPS.cso" });
}


void Scene2d::DrawCircle(GeomData& g, MKMaths::color& c) {
	using namespace MKMaths;
	constexpr UINT vc = 360;
	//vertex verts[vc] = { 0 };
	vertex verts[vc] = { 0 };

	constexpr UINT id = vc * 3;

	UINT Indicies[id];
	UINT ic = 0;

	genIndexBufferTriangle(Indicies, vc, ic);

	genVertexBufferCircle(verts, c, g.data.circle.radius, vc);

	

	Mat4 mat = Mat4::Translate_t(g.data.circle.center_x * meters.x, g.data.circle.center_y * meters.y, 0.0f);


	gfx->BindConstantBuffer_Matrix(mat);
	gfx->BindVertexBuffer(verts, vc);
	gfx->BindIndexBuffer(Indicies, id);


	gfx->Draw(id, dpt::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, { L"VS_WTrans.cso", L"GeneralPS.cso" });
}


static PAIR GetMeterCNum(GeomData& gdata) {
	f32 mag = gdata.data.comp_num.mag;
	f32 arg = gdata.data.comp_num.arg;

	f32 max_x = MKMaths::Abs<f32>(mag * cos(arg));
	f32 max_y = MKMaths::Abs<f32>(mag * sin(arg));



	return { max_x, max_y };
}

static PAIR GetMeterCircle(GeomData& gdata) {
	using namespace MKMaths;
	PAIR Center = { gdata.data.circle.center_x , gdata.data.circle.center_y };
	f32 radius = gdata.data.circle.radius;

	f32 max_x = 0.0f;
	Abs<f32>(Center.first - radius) > Abs<f32>(Center.first + radius) ? max_x = Abs<f32>(Center.first - radius) : max_x = Abs<f32>(Center.first + radius);
	f32 max_y = 0.0f;
	Abs<f32>(Center.second - radius) > Abs<f32>(Center.second + radius) ? max_y = Abs<f32>(Center.second - radius) : max_y = Abs<f32>(Center.second + radius);


	return { max_x, max_y };
}

static PAIR GetMeterHLine(GeomData& gdata) {
	using namespace MKMaths;

	PAIR start = { Abs<f32>(gdata.data.hline.xspos) + 10.0f, Abs<f32>(gdata.data.hline.yspos) + 10.0f };

	return start;
}

Scene2d::~Scene2d() {

}

void Scene2d::DefineTheMeter() {
	f32 ar_x = 1.0f, ar_y = 1.0f;

	f32 _sw = *this->sw - 16.0f;
	f32 _sh = *this->sh - 39.0f;

	_sw > _sh ? ar_x = _sh / _sw : ar_y = _sw / _sh;

	meters.aRx = ar_x;
	meters.aRy = ar_y;

	f32 axw = 0.0f;

	MaxPoint.first > MaxPoint.second ?
		axw = (2.0f * MaxPoint.first)
		:
		axw = (2.0f * MaxPoint.second);

	meters.axiswidth = round(axw);
	meters.x = (1.0f / meters.axiswidth) * ar_x;

	meters.y = (1.0f / meters.axiswidth) * ar_y;

#ifndef NDEBUG
	static int written = false;
	if (!written) {
		Log l;

		l << "Meter values: \n";
		l << meters.x;
		l << meters.y;
		l << "screen measurements: \n";
		l << _sw;
		l << _sh;

		l << "Should not be 0:\n";
		l << meters.axiswidth;

		l << "Should be 1:\n";
		l << meters.y * meters.axiswidth;
		l << "Should be 2/3\n";
		l << meters.x * meters.axiswidth;

		!l;
		written = true;
	}
#endif

}

void Scene2d::GetMeterSize() {
	MaxPoint = { 0,0 };

	for (node* p = list; p != NULL; p = p->next) {
		PAIR m = { 0,0 };
		switch (p->data.gType) {
		case CIRCLE: {
			m = GetMeterCircle(p->data);
		}break;
		case HLINE: {
			m = GetMeterHLine(p->data);
		}break;
		case CNUM: {
			m = GetMeterCNum(p->data);
		} break;
		default: {
			ERR("List err", "List item not found");
		}break;
		}
		if (MaxPoint.first < m.first) {
			MaxPoint.first = m.first;
		}
		if (MaxPoint.second < m.second) {
			MaxPoint.second = m.second;
		}
	}
}


void Scene2d::DrawAxis() {





	//z=1*e^1i


	using namespace MKMaths;
	typedef D3D11_PRIMITIVE_TOPOLOGY dpt;
	//DrawPrincipal axis
	f32 axw = meters.axiswidth;
	vertex PrincipalAxis[4] = {
		{axw * meters.x, 0.0f, 0.0f, 1.0f, 0, 255, 0, 0},
		{-axw * meters.x, 0.0f, 0.0f, 1.0f, 0, 255, 0, 0},
		{0.0f, axw * meters.y, 0.0f, 1.0f, 0, 255, 0, 0},
		{0.0f, -axw * meters.y, 0.0f, 1.0f, 0, 255, 0, 0},
	};
	vertex pa[]{
		{1.0f, 0.0f, 0.0f, 1.0f, 0, 255, 0, 0},
		{-1.0f, 0.0f, 0.0f, 1.0f, 0, 255, 0, 0},
	};
	

	

	gfx->BindVertexBuffer(PrincipalAxis, std::size(PrincipalAxis));
	UINT Indicies[] = {
		0, 1,
		2, 3
	};

	gfx->BindIndexBuffer(Indicies, std::size(Indicies));
	gfx->Draw(std::size(Indicies), dpt::D3D11_PRIMITIVE_TOPOLOGY_LINELIST, {L"VS_NOTRANS.cso", L"GeneralPS.cso"});

	

	//draw generated axis
#ifndef NDEBUG
	static bool written = false;
	if (!written) {
		Log l;
		l << "\n\n\nAxis width:";
		l << meters.axiswidth;
		l << "Contained meters:";
		l << (f32)round((axw / meters.x));
		l << "Think they should be the same:";
		l << (f32)round((axw / meters.y));
		!l;
		for (int i = 0; i < 4; i++) {
			Log l;
			l << "Vertex at index ";
			l << i;
			l << "\n";
			l << PrincipalAxis[i];
			l << "\n";
			!l;
		}
		written = true;
	}
	
#endif


	//accounts for +ve x and y aswell as -ve x and y

	UINT numsections = (((UINT)axw) * 2 * 8);
	vertex* whiteaxis_y = new vertex[numsections];
	UINT* indbuff = (UINT*)malloc(sizeof(vertex) * numsections);
	vertex* whiteaxis_x = new vertex[numsections];

	assert(whiteaxis_x != NULL && whiteaxis_y != NULL && indbuff != NULL);



	for (int i = 1, index = 0; i <= axw; index += 4, i++) {

		whiteaxis_y[index] = { axw * (i / axw) * meters.x, axw * meters.y, 0.0f, 1.0f, 255, 255, 255, 0 };
		whiteaxis_y[index + 1] = { axw * (i / axw) * meters.x, -axw * meters.y, 0.0f, 1.0f, 255, 255, 255, 0 };
		whiteaxis_y[index + 2] = { -axw * (i / axw) * meters.x, axw * meters.y, 0.0f, 1.0f, 255, 255, 255, 0 };
		whiteaxis_y[index + 3] = { -axw * (i / axw) * meters.x, -axw * meters.y, 0.0f, 1.0f, 255, 255, 255, 0 };
	}

	for (int i = 1, index = 0; i <= axw; index += 4, i++) {
		whiteaxis_x[index] = { axw * meters.x, axw * (i / axw) * meters.y, 0.0f, 1.0f, 255, 255, 255, 0 };
		whiteaxis_x[index + 1] = { -axw * meters.x, axw * (i / axw) * meters.y, 0.0f, 1.0f, 255, 255, 255, 0 };
		whiteaxis_x[index + 2] = { axw * meters.x, -axw * (i / axw) * meters.y, 0.0f, 1.0f, 255, 255, 255, 0 };
		whiteaxis_x[index + 3] = { -axw * meters.x, -axw * (i / axw) * meters.y, 0.0f, 1.0f, 255, 255, 255, 0 };
	}

	for (int i = 0; i < numsections; i++) {
		indbuff[i] = i;
	}


	gfx->BindVertexBuffer(whiteaxis_y, numsections);
	gfx->BindIndexBuffer(indbuff, numsections);
	gfx->Draw(numsections, dpt::D3D11_PRIMITIVE_TOPOLOGY_LINELIST, { L"VS_NOTRANS.cso", L"GeneralPS.cso" });
	gfx->BindVertexBuffer(whiteaxis_x, numsections);
	gfx->Draw(numsections, dpt::D3D11_PRIMITIVE_TOPOLOGY_LINELIST, { L"VS_NOTRANS.cso", L"GeneralPS.cso" });


	free(indbuff);
	delete[] whiteaxis_y;
	delete[] whiteaxis_x;

}

//////////////
//3d Stuff below here

void Scene2d::DrawAxis3D() noexcept {
	const auto CAx = colours.at(4);
	colours.pop_back();
	const auto CAy = colours.at(3);
	colours.pop_back();
	const auto CAz = colours.at(2);
	colours.pop_back();
	
	MKMaths::color white = { 255, 255, 255 , 0};

	MKMaths::vertex AxisVerts[6] = {
		//X-Axis
		1.0f, 0.0f, 0.0f, 1.0f, CAx,
		-1.0f, 0.0f, 0.0f, 1.0f, white,
		//Y-Axis
		0.0f, 1.0f, 0.0f, 1.0f, CAy,
		0.0f, -1.0f, 0.0f, 1.0f, white,
		//Z-Axis
		0.0f, 0.0f, 1.0f, 1.0f, CAz,
		0.0f, 0.0f, -1.0f, 1.0f, white
	};

	UINT indBuff[] = {
		// X-Axis
		0, 1,
		// Y-Axis
		2, 3,
		// Z-Axis
		4, 5
	};




	gfx->BindVertexBuffer(AxisVerts, static_cast<UINT>(std::size(AxisVerts)));
	gfx->BindIndexBuffer(indBuff, std::size(indBuff));

	
	gfx->Draw_3D(6, dpt::D3D11_PRIMITIVE_TOPOLOGY_LINELIST, {L"VertexShader.cso", L"GeneralPS.cso"}, meters.aRx, meters.aRy);
	
}

//remove both
#define blue 0, 0, 255 , 0
/*
1x+1y+1z=69
*/


static MKMaths::Mat4 GetPlnTransform(MKMaths::plane& pln) {
	using namespace MKMaths;
	polar p = toPolar(pln);

	struct vec3 {
		f32 x, y, z;
	};

	

	const f32 lambda = (pln.d / ((pln.a * pln.a) + (pln.b * pln.b) + (pln.c * pln.c))) * (1.0f / 1000.0f);
	//multiplied by scale factor to ensure translaton is within bounds 1.0f -> -1.0f
	
	const vec3 cP = { lambda * pln.a , lambda * pln.b, lambda * pln.c };



	Mat4 ret =  Mat4::Rotate3D_y_t(p.phi) * Mat4::Rotate3D_z_t(p.theta);

	ret = ret * Mat4::Translate_t(cP.x, cP.y, cP.z);

	
	return ret;
}


void Scene2d::DrawPlane(MKMaths::plane pln) noexcept {
	using namespace MKMaths;
	vertex plnVerts[] = {
		{0.0f, -1.0f, -1.0f, 1.0f, blue},
		{0.0f, -1.0f, 1.0f, 1.0f, blue},
		{0.0f, 1.0f, -1.0f, 1.0f, blue},
		{0.0f, 1.0f, 1.0f, 1.0f, blue}
	};


	UINT indBuff[] = {
		0, 1, 2, 
		2, 1, 3
	};


	Mat4 t = GetPlnTransform(pln);
#ifndef NDEBUG
	static int i = 0;
	if (i < 3) {
		Log l;
		l << "Transform: \n\n";
		l << t;
		!l;
	}
	i++;
#endif
	//t = Mat4::Identity();
	gfx->BindConstantBuffer_Matrix(t, 1u);

	gfx->BindVertexBuffer(plnVerts, static_cast<UINT>(std::size(plnVerts)));

	gfx->BindIndexBuffer(indBuff, std::size(indBuff));

	gfx->Draw_3D(6, dpt::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, {L"Plane_Shader.cso", L"GeneralPS.cso"}, meters.aRx, meters.aRy);
	
}


void Scene2d::RenderMatSystem() noexcept {
	
	for (node* p = list; p != NULL; p = p->next) {
		if (p->data.gType == MAT) {
			MKMaths::plane pln = {
				p->data.data.mat.a,
				p->data.data.mat.b,
				p->data.data.mat.c,
				p->data.data.mat.d,
			};
			DrawPlane(pln);
		}
	}

	
	
}



void Scene2d::RefreshMeters() noexcept {
	GetMeterSize();
	DefineTheMeter();
}


void Scene2d::GetAR() noexcept {
	if (*sw > *sh) {
		meters.aRx = 1.0f;
		meters.aRy = *sh / *sw;
	}
	else {
		meters.aRy = 1.0f;
		meters.aRx = *sw / *sh;
	}
}


void Scene2d::Render3D() noexcept {
	//RefreshMeters();
	GetAR();
	DrawAxis3D();
	RenderMatSystem();
}


