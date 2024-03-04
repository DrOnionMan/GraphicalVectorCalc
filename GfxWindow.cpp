#include"GfxWindow.h"
#include"MainWindow.h"
#include"MacroUtils.h"
#include"StringUtils.h"


gfxWindow::gfxWindowClass gfxWindow::gfxWindowClass::wndClass;

gfxWindow::gfxWindowClass::gfxWindowClass() noexcept
	: hInst(GetModuleHandle(nullptr)) {
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hCursor = nullptr;
	wc.hIcon = nullptr;
	wc.lpszMenuName = nullptr;
	//HAS TO BE NULL
	wc.hbrBackground = NULL;
	wc.hInstance = GetInstance();
	wc.lpszClassName = GetName();
	wc.style = CS_OWNDC;
	wc.hIconSm = nullptr;
	wc.lpfnWndProc = HandleMsgSetup_gfx;
	RegisterClassEx(&wc);
}
gfxWindow::gfxWindowClass::~gfxWindowClass() {
	UnregisterClass(wndClassName, GetInstance());
}
const char* gfxWindow::gfxWindowClass::GetName() noexcept {
	return wndClassName;
}
HINSTANCE gfxWindow::gfxWindowClass::GetInstance() noexcept {
	return wndClass.hInst;
}
gfxWindow::gfxWindow(int width, int height, const char* name, std::pair<bool*, bool> gfxDim, node* list)
	:
	width(width), height(height), alive(gfxDim.first), m_list(list), Dim3D(gfxDim.second)
{

	//Gets rid of title bar 
	RECT wr = { 0 };
	wr.left = 100;
	wr.right = width + wr.left;
	wr.top = 100;
	wr.bottom = height + wr.top;
	
	//Here we use this pass in the style of the gfxWindow and a boolian to say if we have a 
	//menu (ofc we dont here hence false)
	AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU | WS_MAXIMIZEBOX | WS_SIZEBOX, FALSE);

	screenData.width = (float)(wr.right - wr.left);
	screenData.height = (float)(wr.bottom - wr.top);
	

	hWnd = CreateWindow(
		gfxWindowClass::GetName(), name, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU | WS_MAXIMIZEBOX | WS_SIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top,
		nullptr, nullptr, gfxWindowClass::GetInstance(), this
	);

	if (!hWnd) {
		exit(EXIT_FAILURE);
	}

	pGfx = new Graphics(hWnd, &screenData.width, &screenData.height, m_list, Dim3D);


	ShowWindow(hWnd, SW_SHOWDEFAULT);
}

#include"Logger.h"

void gfxWindow::AdjustgfxWindowRect(LPARAM lp) {
	//shioft rect so top bar not included
	RECT wr = { 0 };
	wr.left = 100;
	wr.right = width + wr.left;
	wr.top = 100;
	wr.bottom = height + wr.top;
	//ajust to new rect
	AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU | WS_MAXIMIZEBOX | WS_SIZEBOX, FALSE);
	//get the rect of my window
	GetWindowRect(hWnd, &wr);
	//f=get sh and sw from this
	//passed to my graphics module as pointers
	screenData.width = (float)(wr.right - wr.left);
	screenData.height = (float)(wr.bottom - wr.top);
	//logging
#ifndef NDEBUG
	Log l;
	l << "-------------------\nSW -> ";
	l << screenData.width;
	l << "\n-------------------\nSH -> ";
	l << screenData.height;
	!l;
#endif
}

//Pretty self explanitory lmao
gfxWindow::~gfxWindow() {
	//obvious cleaup here
	delete pGfx;
	DestroyWindow(hWnd);
	//delete my LL
	DeleteList(m_list);
}


void gfxWindow::SetTitle(const std::string& Title) {
	SetWindowText(hWnd, Title.c_str());	
}


LRESULT WINAPI gfxWindow::HandleMsgSetup_gfx(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept {


	//same as Mainwinodow.cpp
	if (msg == WM_NCCREATE) {
	
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
	
		gfxWindow* const pWnd = static_cast<gfxWindow*>(pCreate->lpCreateParams);
		

		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
		
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&gfxWindow::InvokeMemberFunc_gfx));
		return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT WINAPI gfxWindow::InvokeMemberFunc_gfx(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept {
	
	gfxWindow* const pWnd = reinterpret_cast<gfxWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	
	return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
}

LRESULT gfxWindow::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept {

	

	//Domnt destroy window here as it gets called in distructor
	//this stops us from calling destroywindow twice and causing undefined behavior
	switch (msg) {

	case WM_CLOSE:
		//this is super cursed
		//set the boolian so the window is off
		*alive = false;
		//delete the existing pointer to the class
		delete this;
		break;
	
	case WM_MOUSEMOVE:
	{

		const POINTS pt = MAKEPOINTS(lParam);
		// check if mouse is in window -> normal Mouse move event
		if (pt.x >= 0 && pt.x <= width && pt.y >= 0 && pt.y <= height) {
			mouse.OnMouseMove(pt.x, pt.y);
			//if the isInWindow member = false as we r in wnd then set it to true & call setCapture that Captures mouse
			//Means if mouse leaves wnd were still gonna get mouse move events
			if (!mouse.IsInWindow()) {
				SetCapture(hWnd);
				mouse.OnMouseEnter();
			}
		}
		else {
			//if outside client area mask wParam with the Rbutton and Lbutton macros if those buttons are down this will return
			//True
			if (wParam & (MK_LBUTTON | MK_RBUTTON)) {
				mouse.OnMouseMove(pt.x, pt.y);
			}
			else {
				ReleaseCapture();
				mouse.OnMouseLeave();
			}
		}

		break;
	}
	case WM_MBUTTONDOWN:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnMButtonDown(pt.x, pt.y);
		break;
	}
	case WM_MBUTTONUP:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnMButtonReleased(pt.x, pt.y);
		break;
	}
	case WM_LBUTTONDOWN:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnLeftPressed(pt.x, pt.y);
		break;
	}
	case WM_LBUTTONUP:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnLeftReleased(pt.x, pt.y);
		break;
	}
	case WM_RBUTTONDOWN:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnRightPressed(pt.x, pt.y);
		break;
	}
	case WM_RBUTTONUP:
	{
		//obvs
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnRightReleased(pt.x, pt.y);
		break;
	}
	case WM_MOUSEWHEEL:
	{
		//getting mouse pos as usual
		const POINTS pt = MAKEPOINTS(lParam);
		//have to do this as diff mouses have diff deltas
		const int delta = GET_WHEEL_DELTA_WPARAM(wParam);
		//pass delta in to be handled in the func
		mouse.OnWheelDelta(pt.x, pt.y, delta);

	}
	case WM_KEYDOWN:
		//lparam mask to check if key is held
		if (!(lParam & 0x40000000) || kbd.AutoRepeatIsEnabled()) {
			kbd.OnKeyPressed(static_cast<unsigned char>(wParam));
		}
		break;
	case WM_KEYUP:
		//obvious
		kbd.OnKeyReleased(static_cast<unsigned char>(wParam));
		break;
	case WM_CHAR:
		//wparam contains pressed char
		kbd.OnChar(static_cast<unsigned char>(wParam));
		break;
	case WM_KILLFOCUS:
		kbd.ClearState();
		break;
	case WM_SIZE:
		//get the new width and height of my window
		AdjustgfxWindowRect(lParam);
		if (pGfx) {
			//if my pointer is not null resize my swapchain target
			pGfx->AdjustSwapChainBufferSizes();
		}
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}



std::optional<int> gfxWindow::ProcessMessage() {
	MSG msg;
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
		if (msg.message == WM_QUIT) {
			return msg.wParam;
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return {};
}

Graphics& gfxWindow::GFX() {
	//simple getter
	return *pGfx;
}