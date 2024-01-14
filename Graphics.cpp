#include"Graphics.h"
#include<d3dcompiler.h>
#include<string>
#include<math.h>
#include<DirectXMath.h>
#include <d3d11.h>
#include <vector>
#include"StringUtils.h"
#include<cmath>
#include"LinkedList.h"

#include"Logger.h"
#define M_PI 3.14159265358979323846

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib, "d3dCompiler.lib")
#include<math.h>
#include<fenv.h>

typedef D3D11_PRIMITIVE_TOPOLOGY dpt;
typedef unsigned char uchar;
namespace dx = DirectX;
namespace wrl = Microsoft::WRL;

#define PAIR std::pair<f32, f32>

bool lessthan(node* left, node* right) {
	if (left->data.gType < right->data.gType) {
		return true;
	}

	return false;
}


void genIndexBufferTriangle(UINT* ind, UINT vertexCount, UINT& indexCount) {
	

	const UINT first = indexCount;
	
	assert(ind != NULL);
	ind[indexCount] = first;
	ind[++indexCount] = first + 2;
	ind[++indexCount] = first + 1;
	//printf("%d, %d, %d,\n", first, first + 2, first + 1);
	for (UINT i = first + 3, j = first + 2; i < vertexCount; i++, j++) {
		ind[++indexCount] = first;
		//printf("%d,", first);
		ind[++indexCount] = i;
		//printf(" %d,", i);
		ind[++indexCount] = j;
		//printf(" %d,", j);
		//printf("\n");
	}
	ind[++indexCount] = vertexCount - 1;
	ind[++indexCount] = first;
	ind[++indexCount] = first + 1;
	indexCount++;
	//printf("%d, %d, %d\n", vertexCount - 1, first, first + 1);
	//printf("\n\n---------\nindexCount -> %d\n-----------\n\n", indexCount);
	//printf("\n\n----\nvalue of ind[indexCount] -> %d\n----\n\n", ind[indexCount]);
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



#define CP 3603
#define RAD(x) ((float)((x * M_PI) / 180.0f))

void Scene2d::genVertexBufferCircle( MKMaths::vertex* buffer, MKMaths::color& col, f32 radius, int steps) {
	using namespace MKMaths;
	buffer[0] = {0.0f, 0.0f, 0.0f, 1.0f, col};
	double period = 360.0;
	static int count;
	int j;
	flt i;
	
	for (count = 0, j = 1, i = { 360.0f / (float)steps, 0 }; j < steps; j++, ++i, count++) {
		f32 s = cos(RAD(i.x));
		f32 p = sin(RAD(i.x));
		
		buffer[j] = { (meters.x * radius * cos(RAD(i.x))) ,   (meters.y * radius * sin(RAD(i.x))) , 0.0f, 1.0f, col};
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




//z=1.414*e^1.45i
//|c-(1+2i)|=5
//Arg(c-(1+5i))=1.45
Scene2d::Scene2d(f32 swidth, f32 sheight, node* list, Graphics* gfx) : list(list), gfx(gfx) {
	std::vector<MKMaths::color> colours = { {238, 87, 131, 0},{255, 255, 51, 0}, {0, 255, 255, 0}, {255, 153, 51}, {185, 37, 14, 0} };
	
	quicksort_c(this->list, last_node(this->list), lessthan);
	GetMeterSize();
	DefineTheMeter(swidth, sheight);
	
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


	UINT so = 0, vc = 2;

	void* info[3] = { (void*)linePart, (void*)&so, (void*)&vc };

	(Mat4::strech(meters.x, meters.y, 1.0f) * Mat4::Rotate2D(g.data.comp_num.arg)) * info;

	UINT lib[2] = { 0, 1 };

	gfx->BindVertexBuffer(linePart, 2);
	gfx->BindIndexBuffer(lib, 2);


	gfx->Draw(2, dpt::D3D11_PRIMITIVE_TOPOLOGY_LINELIST);


	vc = 3;
	
	void* tinfo[3] = { (void*)trianglePart, (void*)&so, (void*)&vc };

	(Mat4::strech(meters.x, meters.y, 1.0f) * Mat4::Rotate2D(g.data.comp_num.arg) * Mat4::Translate(g.data.comp_num.mag, 0.0f, 0.0f)) * tinfo;

	UINT tib[3] = {
		0, 2, 1
	};

	gfx->BindVertexBuffer(trianglePart, 3);
	gfx->BindIndexBuffer(tib, 3);


	gfx->Draw(3, dpt::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


}


void Scene2d::DrawHline(GeomData& g, MKMaths::color& c) {
	using namespace MKMaths;
	vertex verts[2] = { 
		{0.0f, 0.0f, 0.0f, 1.0f, c },
		{1.0f, 0.0f, 0.0f, 1.0f, c}
	};
	UINT so = 0;
	UINT vc = 2;
	void* info[3] = { (void*)verts ,(void*)&so, (void*)&vc };

	Mat4::Translate(g.data.hline.xspos * meters.x, g.data.hline.yspos * meters.y, 0.0f) * Mat4::Rotate2D(g.data.hline.arg) * info;

	UINT Indicies[2] = {
		0, 1
	};

	gfx->BindVertexBuffer(verts, 2);
	gfx->BindIndexBuffer(Indicies, 2);


	gfx->Draw(2, dpt::D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
}


void Scene2d::DrawCircle(GeomData& g, MKMaths::color& c) {
	using namespace MKMaths;
	constexpr UINT vc = 360;
	//vertex verts[vc] = { 0 };
	vertex verts[vc] = {0};

	constexpr UINT id = vc * 3;

	UINT Indicies[id];
	UINT ic = 0;

	genIndexBufferTriangle(Indicies, vc, ic);
	
	genVertexBufferCircle(verts, c, g.data.circle.radius, vc);

	UINT so = 0;

	void* info[3] = { (void*)verts,(void*)&so, (void*)&vc};

	Mat4 Transform = Mat4::Translate(g.data.circle.center_x * meters.x, g.data.circle.center_y * meters.y, 0.0f);
	
	Transform * info;

	gfx->BindVertexBuffer(verts, vc);
	gfx->BindIndexBuffer(Indicies, id);


	gfx->Draw(id, dpt::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}


PAIR GetMeterCNum(GeomData& gdata) {
	f32 mag = gdata.data.comp_num.mag;
	f32 arg = gdata.data.comp_num.arg;

	f32 max_x = MKMaths::Abs<f32>(mag * cos(arg));
	f32 max_y = MKMaths::Abs<f32>(mag * sin(arg));

	

	return { max_x, max_y };
}

PAIR GetMeterCircle(GeomData& gdata) {
	using namespace MKMaths;
	PAIR Center = { gdata.data.circle.center_x , gdata.data.circle.center_y };
	f32 radius = gdata.data.circle.radius;

	f32 max_x = 0.0f;
	Abs<f32>(Center.first - radius) > Abs<f32>(Center.first + radius) ? max_x = Abs<f32>(Center.first - radius) : max_x = Abs<f32>(Center.first + radius);
	f32 max_y = 0.0f;
	Abs<f32>(Center.second - radius) > Abs<f32>(Center.second + radius) ? max_y = Abs<f32>(Center.second - radius) : max_y = Abs<f32>(Center.second + radius);


	return { max_x, max_y };
}

PAIR GetMeterHLine(GeomData& gdata) {
	using namespace MKMaths;

	PAIR start = { Abs<f32>(gdata.data.hline.xspos) + 10.0f, Abs<f32>(gdata.data.hline.yspos) + 10.0f };
	/*
	f32 arg = gdata.data.hline.arg;

	f32 mag = 10.0f;

	f32 max_x = MKMaths::Abs<f32>(mag * cos(arg));
	f32 max_y = MKMaths::Abs<f32>(mag * sin(arg));
	*/
	return start;
}

Scene2d::~Scene2d() {

}

void Scene2d::DefineTheMeter(f32 sw, f32 sh) {
	f32 ar_x = 1.0f, ar_y = 1.0f;

	sw > sh ? ar_x = sh / sw : ar_y = sw / sh;
	/*
	f32 segment_width = 0.0f;

	MaxPoint.first > MaxPoint.second ?
		segment_width = 1.0f / (2.0f * MaxPoint.first)
		: 
		segment_width = 1.0f / (2.0f * MaxPoint.second);
	*/
	f32 axw = 0.0f;

	MaxPoint.first > MaxPoint.second ?
		axw = (2.0f * MaxPoint.first)
		:
		axw = (2.0f * MaxPoint.second);

	meters.axiswidth = round(axw);
	meters.x = (1.0f / meters.axiswidth) * ar_x;

	meters.y = (1.0f/meters.axiswidth) * ar_y;

#ifndef NDEBUG
	static int written = false;
	if (!written) {
		Log l;

		l << "Meter values: \n";
		l << meters.x;
		l << meters.y;
		l << "screen measurements: \n";
		l << sw;
		l << sh;

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
	gfx->BindVertexBuffer(PrincipalAxis, 4);
	UINT Indicies[] = {
		0,1,
		2,3
	};
	gfx->BindIndexBuffer(Indicies, 4);
	gfx->Draw(4, dpt::D3D11_PRIMITIVE_TOPOLOGY_LINELIST);



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
		written = true;
	}
#endif

	
	//accounts for +ve x and y aswell as -ve x and y
	
	UINT numsections = (((UINT)axw) * 2 * 8);
	vertex* whiteaxis_y = new vertex[numsections];
	UINT* indbuff = (UINT*)malloc(sizeof(vertex) * numsections);
	vertex* whiteaxis_x = new vertex[numsections];
	
	assert(whiteaxis_x != NULL && whiteaxis_y != NULL && indbuff != NULL );
	
	
	
	for (int i = 1, index = 0; i <= axw; index += 4, i++) {

		whiteaxis_y[index] = { axw * (i / axw) * meters.x, axw * meters.y, 0.0f, 1.0f, 255, 255, 255, 0 };
		whiteaxis_y[index + 1] = { axw * (i / axw) * meters.x, -axw * meters.y, 0.0f, 1.0f, 255, 255, 255, 0 };
		whiteaxis_y[index + 2] = { -axw * (i / axw) * meters.x, axw * meters.y, 0.0f, 1.0f, 255, 255, 255, 0 };
		whiteaxis_y[index + 3] = { -axw * (i / axw) * meters.x, -axw * meters.y, 0.0f, 1.0f, 255, 255, 255, 0 };
	}

	for (int i = 1, index = 0; i <= axw; index += 4, i++) {

		whiteaxis_x[index] =     { axw * meters.x, axw * (i / axw) * meters.y, 0.0f, 1.0f, 255, 255, 255, 0 };
		whiteaxis_x[index + 1] = { -axw * meters.x, axw * (i / axw) * meters.y, 0.0f, 1.0f, 255, 255, 255, 0 };
		whiteaxis_x[index + 2] = { axw  * meters.x, -axw * (i / axw) * meters.y, 0.0f, 1.0f, 255, 255, 255, 0 };
		whiteaxis_x[index + 3] = { -axw  * meters.x, -axw * (i / axw) * meters.y, 0.0f, 1.0f, 255, 255, 255, 0 };
	}
	
	for (int i = 0; i < numsections; i++) {
		indbuff[i] = i;
	}

	

	gfx->BindVertexBuffer(whiteaxis_y, numsections);
	gfx->BindIndexBuffer(indbuff, numsections);
	gfx->Draw(numsections, dpt::D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	gfx->BindVertexBuffer(whiteaxis_x, numsections);
	gfx->Draw(numsections, dpt::D3D11_PRIMITIVE_TOPOLOGY_LINELIST);


	free(indbuff);
	delete[] whiteaxis_y;
	delete[] whiteaxis_x;
	
}




Graphics::Graphics(HWND hWnd, float* width, float* height, node* list) :  vpData({ width, height }), glist(list) {
	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 0;
	sd.BufferDesc.RefreshRate.Denominator = 0;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = hWnd;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	FUNC_ASSERT(D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		D3D11_CREATE_DEVICE_DEBUG,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&sd,
		&pSwap,
		&pDevice,
		nullptr,
		&pContext
	));

	wrl::ComPtr<ID3D11Resource> pBackBuffer;
	FUNC_ASSERT(pSwap->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer));

	FUNC_ASSERT(pDevice->CreateRenderTargetView(
		pBackBuffer.Get(),
		nullptr,
		&pTarget
	));

	D3D11_DEPTH_STENCIL_DESC dsDesc = {};
	dsDesc.DepthEnable = TRUE;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
	wrl::ComPtr<ID3D11DepthStencilState> pDSState;
	FUNC_ASSERT(pDevice->CreateDepthStencilState(&dsDesc, &pDSState));

	pContext->OMSetDepthStencilState(pDSState.Get(), 1u);


	wrl::ComPtr<ID3D11Texture2D> pDepthStencil;
	D3D11_TEXTURE2D_DESC descDepth = {};
	descDepth.Width = *vpData.width - 16;
	descDepth.Height = *vpData.height - 39;
	descDepth.MipLevels = 1u;
	descDepth.ArraySize = 1u;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1u;
	descDepth.SampleDesc.Quality = 0u;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	FUNC_ASSERT(pDevice->CreateTexture2D(&descDepth, nullptr, &pDepthStencil));


	D3D11_DEPTH_STENCIL_VIEW_DESC dsvd = {};
	dsvd.Format = DXGI_FORMAT_D32_FLOAT;
	dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvd.Texture2D.MipSlice = 0u;

	FUNC_ASSERT(pDevice->CreateDepthStencilView(pDepthStencil.Get(), &dsvd, &pDSV));

	

	quicksort_c(glist, last_node(glist), lessthan);
#ifndef NDEBUG
	Log l;

	l << glist;
	!l;

#endif
	pContext->OMSetRenderTargets(1u, pTarget.GetAddressOf(), pDSV.Get());
}



Graphics::~Graphics() {
	MBD("Distructor called");
}


void Graphics::EndFrame() {
	pSwap->Present(1u, 0u);
}

void Graphics::SetShaders(const wchar_t* VertexShaderPath, const wchar_t* PixelShaderPath) {
	


	wrl::ComPtr<ID3D11PixelShader> pPixelShader;
	D3DReadFileToBlob(PixelShaderPath, &pBlob);
	pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader);
	pContext->PSSetShader(pPixelShader.Get(), nullptr, 0u);





	wrl::ComPtr<ID3D11VertexShader> pVertexShader;

	D3DReadFileToBlob(VertexShaderPath, &pBlob);
	pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader);
	pContext->VSSetShader(pVertexShader.Get(), nullptr, 0);

}




void Graphics::BindVertexBuffer(MKMaths::vertex* verts, UINT array_size) {
	wrl::ComPtr<ID3D11Buffer> pVertexBuffer;
	D3D11_BUFFER_DESC bd = {};
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0u;
	bd.MiscFlags = 0u;
	bd.ByteWidth = sizeof(MKMaths::vertex) * array_size;
	bd.StructureByteStride = sizeof(MKMaths::vertex);

	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = verts;

	FUNC_ASSERT(pDevice->CreateBuffer(&bd, &sd, &pVertexBuffer));
	const UINT stride = sizeof(MKMaths::vertex);
	const UINT offset = 0u;
	pContext->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);

}




void Graphics::BindIndexBuffer(UINT* indicies, UINT array_size) {
	wrl::ComPtr<ID3D11Buffer> pIndexBuffer;

	D3D11_BUFFER_DESC ibd = {};
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.CPUAccessFlags = 0u;
	ibd.MiscFlags = 0u;
	ibd.ByteWidth = sizeof(UINT) * array_size;
	ibd.StructureByteStride = sizeof(unsigned short);
	D3D11_SUBRESOURCE_DATA isubData = {};
	isubData.pSysMem = indicies;

	FUNC_ASSERT(pDevice->CreateBuffer(&ibd, &isubData, &pIndexBuffer));

	pContext->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0u);


}



//MKMaths::vertex* VertBuffer, UINT vlength, UINT* IndBuffer, UINT ilength 
void Graphics::Draw(UINT VertexCount,D3D11_PRIMITIVE_TOPOLOGY topology) {
	SetShaders(L"VertShader_2d.cso", L"PixShader_2d.cso");
	D3D11_VIEWPORT vp = {};

	vp.Width = *vpData.width - 16;
	vp.Height = *vpData.height - 39;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;

	pContext->RSSetViewports(1u, &vp);


	wrl::ComPtr<ID3D11InputLayout> pInLay;
	const D3D11_INPUT_ELEMENT_DESC ied[] = {
		{"Position", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0u, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"Colour", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, 16u, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	pDevice->CreateInputLayout(ied, std::size(ied),
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		&pInLay
	);

	pContext->IASetInputLayout(pInLay.Get());

	pContext->OMSetRenderTargets(1u, pTarget.GetAddressOf(), nullptr);

	pContext->IASetPrimitiveTopology(topology);

	pContext->DrawIndexed(VertexCount, 0u, 0u);
}


void Graphics::Render() {
	Scene2d scene(*vpData.width - 16.0f, *vpData.height - 39.0f, glist, this);
	//Drawtest();
}


void Graphics::Drawtest(void) {

	SetShaders(L"VertShader_2d.cso", L"PixShader_2d.cso");
	

	

	MKMaths::vertex verts[] = {
		{0.0f, 1.0f, 0.0f, 1.0f, 0, 255, 0, 0},
		{0.0f, -1.0f, 0.0f, 1.0f, 0, 255, 0, 0},
		{1.0f, 0.0f, 0.0f, 1.0f, 0, 255, 0, 0},
		{-1.0f, 0.0f, 0.0f, 1.0f, 0, 255, 0, 0}
	};


	BindVertexBuffer(verts, 4u);

	UINT Indicies[]{
		0,1,
		2,3
	};

	BindIndexBuffer(Indicies, 4);

	
	

	wrl::ComPtr<ID3D11InputLayout> pInLay;
	const D3D11_INPUT_ELEMENT_DESC ied[] = {
		{"Position", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0u, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"Colour", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, 16u, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	pDevice->CreateInputLayout(ied, (UINT)std::size(ied),
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		&pInLay
	);

	pContext->IASetInputLayout(pInLay.Get());

	pContext->OMSetRenderTargets(1u, pTarget.GetAddressOf(), nullptr);

	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_LINELIST);


	

	


	

	pContext->DrawIndexed(4u, 0u, 0u);

}


void Graphics::DrawTriangle(float angle, float x, float z, const wchar_t* pixelshader) {

	struct Vertex {
		struct {
			float x;
			float y;
			float z;
		}pos;

	};

	const Vertex Verts[] =
	{
		{-1.0f, -1.0f, -1.0f},
		{1.0f, -1.0f, -1.0f},
		{-1.0f, 1.0f, -1.0f},
		{1.0f, 1.0f, -1.0f},
		{-1.0f, -1.0f, 1.0f},
		{1.0f, -1.0f, 1.0f},
		{-1.0f, 1.0f, 1.0f},
		{1.0f, 1.0f, 1.0f},

	};
	wrl::ComPtr<ID3DBlob> pBlob;


	wrl::ComPtr<ID3D11Buffer> pVertexBuffer;
	D3D11_BUFFER_DESC bd = {};
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0u;
	bd.MiscFlags = 0u;
	bd.ByteWidth = sizeof(Verts);
	bd.StructureByteStride = sizeof(Vertex);
	D3D11_SUBRESOURCE_DATA subData = {};
	subData.pSysMem = Verts;
	

	FUNC_ASSERT(pDevice->CreateBuffer(&bd, &subData, &pVertexBuffer));
	const UINT stride = sizeof(Vertex);
	const UINT offset = 0u;
	pContext->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);

	const unsigned short Indicies[]{
		0,2,1,2,3,1,
		1,3,5,3,7,5,
		2,6,3,3,6,7,
		4,5,7,4,7,6,
		0,4,2,2,4,6,
		0,1,4,1,5,4
	};
	wrl::ComPtr<ID3D11Buffer> pIndexBuffer;

	D3D11_BUFFER_DESC ibd = {};
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.CPUAccessFlags = 0u;
	ibd.MiscFlags = 0u;
	ibd.ByteWidth = sizeof(Indicies);
	ibd.StructureByteStride = sizeof(unsigned short);
	D3D11_SUBRESOURCE_DATA isubData = {};
	isubData.pSysMem = Indicies;

	FUNC_ASSERT(pDevice->CreateBuffer(&ibd, &isubData, &pIndexBuffer));

	pContext->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);

	


	struct ConstantBuffer {
		dx::XMMATRIX transform;
	};

	const ConstantBuffer cb = {
		{
			dx::XMMatrixTranspose(
				dx::XMMatrixRotationY(angle) *
				dx::XMMatrixRotationZ(angle) *
				dx::XMMatrixTranslation(x, 0.0f, z + 4.0f) *
				dx::XMMatrixPerspectiveLH(1.0f, vpData.getaspectRatio() ,0.5f, 10.0f)
			)  
		}
	};

	wrl::ComPtr<ID3D11Buffer> pConstBuffer;
	D3D11_BUFFER_DESC cbd = {};
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.Usage = D3D11_USAGE_DYNAMIC;
	cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbd.MiscFlags = 0u;
	cbd.ByteWidth = sizeof(cb);
	cbd.StructureByteStride = 0u;
	D3D11_SUBRESOURCE_DATA csd = {};
	csd.pSysMem = &cb;
	FUNC_ASSERT(pDevice->CreateBuffer(&cbd, &csd, &pConstBuffer));
	pContext->VSSetConstantBuffers(0u, 1u, pConstBuffer.GetAddressOf());

	struct ConstBuffer2 {
		struct {
			float r;
			float g;
			float b;
			float a;
		} face_colors[6];
	};

	const ConstBuffer2 cb2 = {
		{
			{1.0f, 0.0f, 1.0f},
			{1.0f, 0.0f, 0.0f},
			{0.0f, 1.0f, 0.0f},
			{0.0f, 0.0f, 1.0f},
			{1.0f, 1.0f, 0.0f},
			{0.0f, 1.0f, 1.0f},
		}
	};

	wrl::ComPtr<ID3D11Buffer> pConstBuffer2;
	D3D11_BUFFER_DESC cbd2 = {};
	cbd2.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd2.Usage = D3D11_USAGE_DEFAULT;
	cbd2.CPUAccessFlags = 0u;
	cbd2.MiscFlags = 0u;
	cbd2.ByteWidth = sizeof(cb2);
	cbd2.StructureByteStride = 0u;
	D3D11_SUBRESOURCE_DATA csd2 = {};
	csd2.pSysMem = &cb2;
	FUNC_ASSERT(pDevice->CreateBuffer(&cbd2, &csd2, &pConstBuffer2));
	pContext->PSSetConstantBuffers(0u, 1u, pConstBuffer2.GetAddressOf());

	wrl::ComPtr<ID3D11PixelShader>  pPixShade;
	FUNC_ASSERT(D3DReadFileToBlob(pixelshader, &pBlob));
	FUNC_ASSERT(pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixShade));
	pContext->PSSetShader(pPixShade.Get(), nullptr, 0u);


	wrl::ComPtr<ID3D11VertexShader> pVertShdr;

	FUNC_ASSERT(D3DReadFileToBlob(L"VertexShader.cso", &pBlob));
	FUNC_ASSERT(pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertShdr));

	pContext->VSSetShader(pVertShdr.Get(), 0, 0);






	wrl::ComPtr<ID3D11InputLayout> pInputLayout;
	const D3D11_INPUT_ELEMENT_DESC ied[] = {
		{"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0 ,D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	FUNC_ASSERT(pDevice->CreateInputLayout(ied, (UINT)std::size(ied), pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &pInputLayout));

	pContext->IASetInputLayout(pInputLayout.Get());





	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);



	D3D11_VIEWPORT vp = {};
	
	vp.Width = *vpData.width;
	vp.Height = *vpData.height;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	
	pContext->RSSetViewports(1u, &vp);


	pContext->DrawIndexed((UINT)std::size(Indicies), 0u, 0u);
}


