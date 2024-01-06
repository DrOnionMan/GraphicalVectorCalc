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

typedef unsigned char uchar;
namespace dx = DirectX;
namespace wrl = Microsoft::WRL;

#define PAIR std::pair<f32, f32>

Scene2d::Scene2d(f32 swidth, f32 sheight, node* list) : list(list) {
	GetMeterSize();
	DefineTheMeter(swidth, sheight);
	
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

	f32 segment_width = 0.0f;

	MaxPoint.first > MaxPoint.second ?
		segment_width = 1 / (2 * MaxPoint.first)
		: 
		segment_width = 1 / (2 * MaxPoint.second);

	MaxPoint.first > MaxPoint.second ?
		meters.axiswidth = (2 * MaxPoint.first)
		:
		meters.axiswidth = (2 * MaxPoint.second);


	meters.x = segment_width * ar_x;

	meters.y = segment_width * ar_y;

#ifndef NDEBUG
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
	descDepth.Width = *vpData.width;
	descDepth.Height = *vpData.height;
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

	vpData.MakeNormal();

	Scene2d swag(*vpData.width, *vpData.height, glist);
	

	
	
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


void Graphics::DrawAxis2D(void) {

	SetShaders(L"VertexShader.cso", L"PixelShader.cso");

	struct vert {
		float x;
		float y;
	};

	const vert verts[] = {
		{0.0f, 1.0f},
		{0.0f, -1.0f},
		{1.0f, 0.0f},
		{-1.0f, 0.0f}
	};

	wrl::ComPtr<ID3D11Buffer> pVertexBuffer;
	D3D11_BUFFER_DESC bd = {};
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0u;
	bd.MiscFlags = 0u;
	bd.ByteWidth = sizeof(verts);
	bd.StructureByteStride = sizeof(vert);


	const unsigned short Indicies[]{
		0,1,
		2,3
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




	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = verts;

	pDevice->CreateBuffer(&bd, &sd, &pVertexBuffer);
	const UINT stride = sizeof(vert);
	const UINT offset = 0u;
	pContext->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);

	
	

	wrl::ComPtr<ID3D11InputLayout> pInLay;
	const D3D11_INPUT_ELEMENT_DESC ied[] = {
		{"Position", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	pDevice->CreateInputLayout(ied, (UINT)std::size(ied),
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		&pInLay
	);

	pContext->IASetInputLayout(pInLay.Get());

	pContext->OMSetRenderTargets(1u, pTarget.GetAddressOf(), nullptr);

	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_LINELIST);


	

	


	D3D11_VIEWPORT vp = {};
	
	vp.Width = *vpData.width - 16;
	vp.Height = *vpData.height - 39;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;

	pContext->RSSetViewports(1u, &vp);


	pContext->DrawIndexed((UINT)std::size(Indicies), 0u, 0u);

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


