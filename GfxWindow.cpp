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
gfxWindow::gfxWindow(int width, int height, const char* name, bool *isalive, node* list)
	:
	width(width), height(height), alive(isalive), m_list(list)
{

	//Gets rid of title bar 
	RECT wr = { 0 };
	wr.left = 100;
	wr.right = width + wr.left;
	wr.top = 100;
	wr.bottom = height + wr.top;
	
	//Here we use this pass in the style of the gfxWindow and a boolian to say if we have a menu (ofc we dont here hence false)
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


	//pGfx = std::make_unique<Graphics>(hWnd, &screenData.width, &screenData.height, m_list);
	pGfx = new Graphics(hWnd, &screenData.width, &screenData.height, m_list);
	ShowWindow(hWnd, SW_SHOWDEFAULT);
}



void gfxWindow::AdjustgfxWindowRect(LPARAM lp) {
	RECT wr = { 0 };
	wr.left = 100;
	wr.right = width + wr.left;
	wr.top = 100;
	wr.bottom = height + wr.top;
	AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU | WS_MAXIMIZEBOX | WS_SIZEBOX, FALSE);
	screenData.width = (float)(wr.right - wr.left);
	screenData.height = (float)(wr.bottom - wr.top);
	
}

//Pretty self explanitory lmao
gfxWindow::~gfxWindow() {
	delete pGfx;
	DestroyWindow(hWnd);
	DeleteList(m_list);
	
}


void gfxWindow::SetTitle(const std::string& Title) {
	SetWindowText(hWnd, Title.c_str());	
}
LRESULT WINAPI gfxWindow::HandleMsgSetup_gfx(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept {



	if (msg == WM_NCCREATE) {
	
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
	
		gfxWindow* const pWnd = static_cast<gfxWindow*>(pCreate->lpCreateParams);
		//pWnd->pGfx = new Graphics(hWnd);

		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
		
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&gfxWindow::InvokeMemberFunc_gfx));
		return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}
//This function is just an adapter Converts from win32api to my c++ code (member call convention)
LRESULT WINAPI gfxWindow::InvokeMemberFunc_gfx(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept {
	
	gfxWindow* const pWnd = reinterpret_cast<gfxWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	
	return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
}
LRESULT gfxWindow::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept {

	

	//Domnt destroy window here as it gets called in distructor
	//this stops us from calling destroywindow twice and causing undefined behavior
	switch (msg) {

	case WM_CLOSE:
		*alive = false;
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
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnRightReleased(pt.x, pt.y);
		break;
	}
	case WM_MOUSEWHEEL:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		const int delta = GET_WHEEL_DELTA_WPARAM(wParam);
		mouse.OnWheelDelta(pt.x, pt.y, delta);

	}
	case WM_KEYDOWN:
		if (!(lParam & 0x40000000) || kbd.AutoRepeatIsEnabled()) {
			kbd.OnKeyPressed(static_cast<unsigned char>(wParam));
		}
		break;
	case WM_KEYUP:
		kbd.OnKeyReleased(static_cast<unsigned char>(wParam));
		break;
	case WM_CHAR:
		kbd.OnChar(static_cast<unsigned char>(wParam));
		break;
	case WM_KILLFOCUS:
		kbd.ClearState();
		break;
	case WM_SIZE:
		AdjustgfxWindowRect(lParam);
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
	return *pGfx;
}