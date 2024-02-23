#pragma once
#include"MKMath.h"
#include"LinkedList.h"
#include"Graphics.h"
#include<vector>

class Scene2d {
public:
	Scene2d(f32* swidth, f32* sheight, node* list, Graphics* __restrict gfx);
	~Scene2d();
	Scene2d(const Scene2d&) = delete;
	Scene2d& operator=(const Scene2d&) = delete;
	

	void Render() noexcept;

	std::pair<f32, f32> MaxPoint;


	void Render3D() noexcept;
	

	Graphics* gfx;
	

	struct meter {
		f32 x;
		f32 y;
		f32 axiswidth;
		f32 aRx, aRy;
	};

	f32* sw, * sh;
	meter meters;

private:
	void DefineTheMeter();
	void DrawAxis();
	void DrawCircle(GeomData& g, MKMaths::color& c);
	void genVertexBufferCircle(MKMaths::vertex* buffer, MKMaths::color& col, f32 radius, int steps);
	void DrawHline(GeomData& g, MKMaths::color& c);
	void DrawCNum(GeomData& g, MKMaths::color& c);
	void GetMeterSize();
	void DrawAxis3D() noexcept;
	void RenderMatSystem() noexcept;
	void RefreshMeters() noexcept;
	void DrawPlane(MKMaths::plane pln) noexcept;
	void GetAR() noexcept;

	std::vector<MKMaths::color> colours = { {238, 87, 131, 0},{255, 255, 51, 0}, {0, 255, 255, 0}, {255, 153, 51}, {185, 37, 14, 0} };

	
	node* list;
	bool Dim;


	



};