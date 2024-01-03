#pragma once

#include<assert.h>
#include<d3d11.h>
#include<wrl.h>
#include"StringUtils.h"
#include"LinkedList.h"


#ifdef NDEBUG
#undef _DEBUG
#define FUNC_ASSERT(x) (x)
#define MBD(x) (x)
#endif


#ifdef _DEBUG
#define FUNC_ASSERT(x) (assert(x == S_OK))
#define MBD(x) (MessageBox(NULL, x, nullptr, MB_OK))
#endif



class Graphics{
public:

	Graphics(HWND hWnd, float* width, float* height, void* list);
	Graphics(const Graphics&) = delete;
	~Graphics();
	Graphics& operator=(const Graphics&) = delete;
	void EndFrame();
	void ClearBuffer(float red, float green, float blue) noexcept {
		const float color[] = { red, green, blue, 1.0f };
		pContext->ClearRenderTargetView(pTarget.Get(), color);
		pContext->ClearDepthStencilView(pDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
	}
	void DrawTriangle(float angle, float x, float z, const wchar_t* pixelshader);
	void PrintVp(void) {
		PrintNums<double>(2, *vpData.height, *vpData.width);
	}
	void DrawAxis2D(void);

private:
	struct {
		float *width; float *height; 
		float getaspectRatio() {
			return *height / *width;
		}
	}vpData;
	node* glist;
	Microsoft::WRL::ComPtr<ID3D11Device> pDevice;
	Microsoft::WRL::ComPtr<IDXGISwapChain> pSwap;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pTarget;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDSV;
};