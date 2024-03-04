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
#include"DebugDX.h"

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib, "d3dCompiler.lib")
#include<math.h>
#include<fenv.h>
#include"Scene2d.h"

typedef D3D11_PRIMITIVE_TOPOLOGY dpt;
typedef unsigned char uchar;
namespace dx = DirectX;
namespace wrl = Microsoft::WRL;






Graphics::Graphics(HWND hWnd, float* width, float* height, node* list, bool Dim) 
	: vpData({ width, height }), glist(list), _Dim(Dim),DTS(-5.0f), Arg_H(0.0f),Arg_V(0.0f),SF(7u) {
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

	//Debug info obviously doesnt need to be included in Release
#ifndef NDEBUG
	constexpr auto DevFlag = D3D11_CREATE_DEVICE_DEBUG;
#else
	constexpr auto DevFlag = 0;
#endif

	FUNC_ASSERT(D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		DevFlag,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&sd,
		&pSwap,
		&pDevice,
		nullptr,
		&pContext
	));


	//TO DISABLE BACK FACE CULLING
	D3D11_RASTERIZER_DESC rasterizerDesc = {};
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_NONE;
	
	rasterizerDesc.FrontCounterClockwise = FALSE; 
	ID3D11RasterizerState* noCullRasterizerState;
	FUNC_ASSERT(pDevice->CreateRasterizerState(&rasterizerDesc, &noCullRasterizerState));

	//bind to pipeline
	pContext->RSSetState(noCullRasterizerState);



	noCullRasterizerState->Release();

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


	
	D3D11_TEXTURE2D_DESC descDepth = {};
	descDepth.Width = *vpData.width - 16.0f;
	descDepth.Height = *vpData.height - 39.0f;
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






	
#ifndef NDEBUG
	Log l;

	l << glist;
	!l;

#endif
	pContext->OMSetRenderTargets(1u, pTarget.GetAddressOf(), pDSV.Get());
}



Graphics::~Graphics() {
	
}


void Graphics::EndFrame() {
	FUNC_ASSERT(pSwap->Present(1u, 0u));
}

void Graphics::SetShaders(const wchar_t* VertexShaderPath, const wchar_t* PixelShaderPath) {
	//local blob as when using the one in the class i run into issues.
	// 
	//bind pixel shader
	wrl::ComPtr<ID3D11PixelShader> pPixelShader;
	FUNC_ASSERT(D3DReadFileToBlob(PixelShaderPath, &pBlob));
	FUNC_ASSERT(pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader));
	pContext->PSSetShader(pPixelShader.Get(), nullptr, 0u);



	//bind vertex shader
	wrl::ComPtr<ID3D11VertexShader> pVertexShader;
	FUNC_ASSERT(D3DReadFileToBlob(VertexShaderPath, &pBlob));
	FUNC_ASSERT(pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader));
	pContext->VSSetShader(pVertexShader.Get(), nullptr, 0);
}




void Graphics::BindVertexBuffer(MKMaths::vertex* verts, UINT array_size) {
	//create ptr
	wrl::ComPtr<ID3D11Buffer> pVertexBuffer;
	D3D11_BUFFER_DESC bd = {};
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0u;
	bd.MiscFlags = 0u;
	//obvious
	bd.ByteWidth = sizeof(MKMaths::vertex) * array_size;
	bd.StructureByteStride = sizeof(MKMaths::vertex);

	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = verts;

	FUNC_ASSERT(pDevice->CreateBuffer(&bd, &sd, &pVertexBuffer));
	const UINT stride = sizeof(MKMaths::vertex);
	const UINT offset = 0u;
	//set the buff
	pContext->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);

}




void Graphics::BindIndexBuffer(UINT* indicies, UINT array_size) {
	//ptr
	wrl::ComPtr<ID3D11Buffer> pIndexBuffer;

	D3D11_BUFFER_DESC ibd = {};
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.CPUAccessFlags = 0u;
	ibd.MiscFlags = 0u;
	//obvs
	ibd.ByteWidth = sizeof(UINT) * array_size;
	ibd.StructureByteStride = sizeof(unsigned short);
	D3D11_SUBRESOURCE_DATA isubData = {};
	isubData.pSysMem = indicies;

	FUNC_ASSERT(pDevice->CreateBuffer(&ibd, &isubData, &pIndexBuffer));
	//using 32 bit uint
	pContext->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0u);


}

void Graphics::BindConstantBuffer_Matrix(const MKMaths::Mat4& matrix) {
	//float4x4 for the shader
	struct ConstantBuffer {
		f32 ShaderFmt[4][4];
	};

	ConstantBuffer cBuff = { 0 };
	//transpose
	//OVERLOAD
	!matrix;


	//fill cbuff with matrix info
	matrix.ToShader(cBuff.ShaderFmt);



	wrl::ComPtr<ID3D11Buffer> pConstantBuffer;
	D3D11_BUFFER_DESC cbd = {};
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.Usage = D3D11_USAGE_DYNAMIC;
	cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbd.MiscFlags = 0u;
	cbd.ByteWidth = sizeof(cBuff);
	cbd.StructureByteStride = 0u;
	D3D11_SUBRESOURCE_DATA csd = {};
	csd.pSysMem = &cBuff;
	FUNC_ASSERT(pDevice->CreateBuffer(&cbd, &csd, &pConstantBuffer));

	// bind constant buffer to vertex shader
	pContext->VSSetConstantBuffers(0u, 1u, pConstantBuffer.GetAddressOf());
	
}


void Graphics::BindConstantBuffer_Matrix(const MKMaths::Mat4& matrix, const UINT slot) {
	//Annoying float4x4 conversion on cpu side
	struct ConstantBuffer {
		f32 ShaderFmt[4][4];
	};

	ConstantBuffer cBuff = { 0 };

	!matrix;



	matrix.ToShader(cBuff.ShaderFmt);



	wrl::ComPtr<ID3D11Buffer> pConstantBuffer;
	D3D11_BUFFER_DESC cbd = {};
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.Usage = D3D11_USAGE_DYNAMIC;
	cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbd.MiscFlags = 0u;
	cbd.ByteWidth = sizeof(cBuff);
	cbd.StructureByteStride = 0u;
	D3D11_SUBRESOURCE_DATA csd = {};
	csd.pSysMem = &cBuff;
	FUNC_ASSERT(pDevice->CreateBuffer(&cbd, &csd, &pConstantBuffer));

	// bind constant buffer to vertex shader
	pContext->VSSetConstantBuffers(slot, 1u, pConstantBuffer.GetAddressOf());

}

void Graphics::BindConstantBuffer_DXMatrix(DirectX::XMMATRIX& transform) {
	struct ConstantBuffer {
		dx::XMMATRIX transform;
	};

	ConstantBuffer cBuff = {
		dx::XMMatrixTranspose(transform)
	};





	wrl::ComPtr<ID3D11Buffer> pConstantBuffer;
	D3D11_BUFFER_DESC cbd = {};
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.Usage = D3D11_USAGE_DYNAMIC;
	cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbd.MiscFlags = 0u;
	cbd.ByteWidth = sizeof(cBuff);
	cbd.StructureByteStride = 0u;
	D3D11_SUBRESOURCE_DATA csd = {};
	csd.pSysMem = &cBuff;
	FUNC_ASSERT(pDevice->CreateBuffer(&cbd, &csd, &pConstantBuffer));

	// bind constant buffer to vertex shader
	pContext->VSSetConstantBuffers(0u, 1u, pConstantBuffer.GetAddressOf());
}


D3D11_VIEWPORT* Graphics::GetMyVP(void) const noexcept {
	//macro coz i cba writing this out
#define VP D3D11_VIEWPORT
	//dynamic allocation
	VP* vp = (VP*)malloc(sizeof(VP));
	//am i null???
	if (vp) {
		//this has the same eg=ffect as calloc, could use calloc
		//but i prefer malloc
		//this is coz you should never use ininitialised memory in a struct ever
		//can cause undefined behaviour
		ZeroMemory(vp, sizeof(VP));
		vp->MinDepth = 0;
		vp->MaxDepth = 1;
		vp->TopLeftX = 0;
		vp->TopLeftY = 0;
		//get width and height as an integer
		UINT vH = (UINT)*vpData.height;
		UINT vW = (UINT)*vpData.width;
		//WHY DOESNT MSDN MENTION THIS
		//to draw lines consistently the WINDOW WIDTH AND HEIGHT MUST BE EVEN
		//WHYYYYYYYYYYYYYYY
		const bool isEven = (vH % 2 == 0);
		//had abit of fun here
		const bool isEvenStrikesBack = (vW % 2 == 0);
		//handles the cases for even and odd
		if (isEven && isEvenStrikesBack) {
			vp->Width = vW;
			vp->Height = vH;
			return vp;
		}
		else if (isEven) {
			vp->Width = vW - 1u;
			vp->Height = vH;
			return vp;
		}
		else if (isEvenStrikesBack) {
			vp->Width = vW;
			vp->Height = vH - 1u;
			return vp;
		}
		else {
			vp->Width = vW - 1;
			vp->Height = vH - 1u;
			return vp;
		}
	}
	else {
		//obvious NULLPTR return if malloc fails
		return nullptr;
	}
#undef VP
}


void Graphics::Draw(UINT VertexCount, D3D11_PRIMITIVE_TOPOLOGY topology,
	std::pair<const wchar_t*, const wchar_t*> shaders) {
	//func is 4 2d stuff
	//set shaders
	SetShaders(shaders.first, shaders.second);
	//get vp & bind
	if(D3D11_VIEWPORT* vp = GetMyVP()){
		pContext->RSSetViewports(1u, vp);
		free(vp);
	}



	wrl::ComPtr<ID3D11InputLayout> pInLay;
	const D3D11_INPUT_ELEMENT_DESC ied[] = {
		{"Position", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0u, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		//offset set for col info as its passed in my Vertex buffer
		{"Colour", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, 16u, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	FUNC_ASSERT(pDevice->CreateInputLayout(ied, std::size(ied),
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		&pInLay
	));


	pContext->IASetInputLayout(pInLay.Get());

	pContext->OMSetRenderTargets(1u, pTarget.GetAddressOf(), nullptr);

	pContext->IASetPrimitiveTopology(topology);
	//draw :)
	pContext->DrawIndexed(VertexCount, 0u, 0u);
}




void Graphics::Draw_3D(UINT VertexCount, D3D11_PRIMITIVE_TOPOLOGY topology,
	std::pair<const wchar_t*, const wchar_t*> shaders, f32 VW, f32 VH) noexcept {
	using namespace MKMaths;
	//set those shaders
	SetShaders(shaders.first, shaders.second);

	//perspective transform
	//OVERLOAD
	MKMaths::Mat4 mat =
		Mat4::Rotate3D_y_t(Arg_H) *
		Mat4::Rotate3D_x_t(Arg_V) *
		Mat4::Translate_t(0, 0, DTS) *
		Mat4::PerspectiveRH(VW, VH, 0.5f, 10.0f);
	BindConstantBuffer_Matrix(mat, 0u);//bound to slot 0 in VS


	//get vp & bind
	if (D3D11_VIEWPORT* vp = GetMyVP()) {
		pContext->RSSetViewports(1u, vp);
		free(vp);
	}

	


	wrl::ComPtr<ID3D11InputLayout> pInLay;
	const D3D11_INPUT_ELEMENT_DESC ied[] = {
		{"Position", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0u, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		//same as 2d
		{"Colour", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, 16u, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	FUNC_ASSERT(pDevice->CreateInputLayout(ied, std::size(ied),
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		&pInLay
	));

	pContext->IASetInputLayout(pInLay.Get());

	pContext->OMSetRenderTargets(1u, pTarget.GetAddressOf(), pDSV.Get());

	pContext->IASetPrimitiveTopology(topology);

	pContext->DrawIndexed(VertexCount, 0u, 0u);
}



void Graphics::Render(void) {
	//hate this but void* cast was bugging
	Scene2d scene(vpData.width, vpData.height, glist, this, &SF);
	//if 3d?
	if (_Dim) {
		scene.Render3D();
		return;
	}
	//else
	scene.Render();
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
	FUNC_ASSERT(pDevice->CreateInputLayout(ied, (UINT)std::size(ied),
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		&pInLay
	));

	pContext->IASetInputLayout(pInLay.Get());

	pContext->OMSetRenderTargets(1u, pTarget.GetAddressOf(), nullptr);

	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_LINELIST);









	pContext->DrawIndexed(4u, 0u, 0u);

}



//Cringe function but has to be done
void Graphics::AdjustSwapChainBufferSizes() noexcept {

	//essentially get rid of everything related to the target
	pContext->OMSetRenderTargets(0, 0, 0);
	pTarget->Release();
	pDSV.Reset();

	//make the target have the same dimentions as my window
	FUNC_ASSERT(pSwap->ResizeBuffers(0, *vpData.width, *vpData.height, DXGI_FORMAT_UNKNOWN, 0));
	//comptr was breaking so this will have to do
	ID3D11Texture2D* pBuff = NULL;
	//get the back buffer
	FUNC_ASSERT(pSwap->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBuff));
	assert(pBuff != NULL);
	//create new target
	FUNC_ASSERT(pDevice->CreateRenderTargetView(pBuff, 0, pTarget.GetAddressOf()));
	//release temp buffer
	pBuff->Release();


	//recreate the DSV as it all gets lost
	//why havnt they created a function for this?????????
	D3D11_TEXTURE2D_DESC depthStencilDesc = {};
	pDepthStencil->GetDesc(&depthStencilDesc);
	//pointers to the width and height off the window
	depthStencilDesc.Width = *vpData.width;
	depthStencilDesc.Height = *vpData.height;
	//same as in class constructor
	FUNC_ASSERT(pDevice->CreateTexture2D(&depthStencilDesc, nullptr, &pDepthStencil));
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
	depthStencilViewDesc.Format = depthStencilDesc.Format;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;
	FUNC_ASSERT(pDevice->CreateDepthStencilView(pDepthStencil.Get(), &depthStencilViewDesc, &pDSV));
}




