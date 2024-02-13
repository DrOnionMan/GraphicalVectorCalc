#pragma once
#include"MKMath.h"
#include"LinkedList.h"
#include"Graphics.h"

class Scene2d {
public:
	Scene2d(f32* swidth, f32* sheight, node* list, Graphics* __restrict gfx);
	~Scene2d();
	Scene2d(const Scene2d&) = delete;
	Scene2d& operator=(const Scene2d&) = delete;


	void Render() noexcept;

	std::pair<f32, f32> MaxPoint;

	void GetMeterSize();

	MKMaths::vertex* TriangleBuffer;
	MKMaths::vertex* LineBuffer;
	UINT* LineIndexBuffer;
	UINT* TriangleIndexBuffer;
private:
	void DefineTheMeter();
	void DrawAxis();
	void DrawCircle(GeomData& g, MKMaths::color& c);
	void genVertexBufferCircle(MKMaths::vertex* buffer, MKMaths::color& col, f32 radius, int steps);
	void DrawHline(GeomData& g, MKMaths::color& c);
	void DrawCNum(GeomData& g, MKMaths::color& c);


	Graphics* gfx;
	node* list;
	UINT cVertexCount;
	UINT IndexCount;
	UINT LStartOffset;
	UINT TStartOffset;
	struct meter {
		f32 x;
		f32 y;
		f32 axiswidth;
		f32 aRx, aRy;
	};

	f32* sw, *sh;

	meter meters;
};