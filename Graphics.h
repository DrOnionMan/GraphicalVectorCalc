#pragma once

#include<assert.h>
#include<d3d11.h>
#include<wrl.h>
#include"StringUtils.h"
#include"LinkedList.h"
#include"MKMath.h"

#include"DebugDX.h"


#include<directxMath.h>



class Graphics{
public:

	Graphics(HWND hWnd, float* width, float* height, node* list, bool Dim);
	Graphics(const Graphics&) = delete;
	~Graphics();
	Graphics& operator=(const Graphics&) = delete;
	void EndFrame();
	void ClearBuffer(float red, float green, float blue) noexcept {
		const float color[] = { red, green, blue, 1.0f };
		pContext->ClearRenderTargetView(pTarget.Get(), color);
		pContext->ClearDepthStencilView(pDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
	}
	//Legacy remove
	
	//legacy remove
	void PrintVp(void) {
		PrintNums<double>(2, *vpData.height, *vpData.width);
	}

	void Draw(UINT VertexCount, D3D11_PRIMITIVE_TOPOLOGY topology,
		std::pair<const wchar_t*, const wchar_t*> shaders);
	void Draw_3D(UINT VertexCount, D3D11_PRIMITIVE_TOPOLOGY topology,
		std::pair<const wchar_t*, const wchar_t*> shaders, f32 VW, f32 VH) noexcept;
	void Drawtest(void);
	void SetShaders(const wchar_t* VertexShaderPath, const wchar_t* PixelShaderPath);
	void BindVertexBuffer(MKMaths::vertex* verts, UINT array_size);
	void BindIndexBuffer(UINT* indicies, UINT array_size);
	void BindConstantBuffer_Matrix(const MKMaths::Mat4& matrix);
	void BindConstantBuffer_DXMatrix(DirectX::XMMATRIX& transform);
	void BindConstantBuffer_Matrix(const MKMaths::Mat4& matrix, const UINT slot);
	void AdjustBufferSize(){}
	void AdjustSwapChainBufferSizes() noexcept;
	void Render(void);


	//MAKE SURE ALLOCATED BUFFER IS DELETED
	D3D11_VIEWPORT* GetMyVP(void) const noexcept;


	f32 DTS;
	f32 Arg_H;
	f32 Arg_V;


private:
public:


	struct {
		float *width; float *height; 
		float getaspectRatio() {
			return *height / *width;
		}
		
	}vpData;
	node* glist;



	

	bool _Dim;
	
	Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
	Microsoft::WRL::ComPtr<ID3D11Device> pDevice;
	Microsoft::WRL::ComPtr<IDXGISwapChain> pSwap;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pTarget;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDSV;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> pDepthStencil;

};


