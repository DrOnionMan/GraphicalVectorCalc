#include"MainWindow.h"
#include"MenuMacroDef.h"
Window::MainWindowClass Window::MainWindowClass::wndClass;





Window::MainWindowClass::MainWindowClass() noexcept
	: hInst(GetModuleHandle(nullptr)) {
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hCursor = nullptr;
	wc.hIcon = nullptr;
	wc.lpszMenuName = nullptr;
	wc.hbrBackground = nullptr;
	wc.hInstance = GetInstance();
	wc.lpszClassName = GetName();
	wc.style = CS_OWNDC;
	wc.hIconSm = nullptr;
	wc.lpfnWndProc = HandleMsgSetup;
	RegisterClassEx(&wc);
}
Window::MainWindowClass::~MainWindowClass() {
	UnregisterClass(wndClassName, GetInstance());
}
const char* Window::MainWindowClass::GetName() noexcept {
	return wndClassName;
}
HINSTANCE Window::MainWindowClass::GetInstance() noexcept {
	return wndClass.hInst;
}

void Window::Menus::CreateMainMenu() {
	SetArgandDiagramTopic();
}


Window::Menus::Menus() : Menu(CreateMenu()) {
	CreateMainMenu();
}

Window::Menus::~Menus() {
	
}

void Window::Menus::SetArgandDiagramTopic() {
	HMENU Argand = CreateMenu();

	HMENU InputType = CreateMenu();
	//Defining The Input Types For the Argand diagrams
	AppendMenu(InputType, MF_STRING, A_BI, "a + bi");
	AppendMenu(InputType, MF_STRING, EI, "r*e^i");
	AppendMenu(InputType, MF_STRING, MODARG, "r(cos(x) + isin(x))");


	AppendMenu(Argand, MF_STRING, GET_DIFF_FORM_ARGAND, "Format Converter");
	AppendMenu(Argand, MF_STRING, DRAW_ARGAND_DIAGRAM, "Argand Diagram Drawer");
	AppendMenu(Argand, MF_SEPARATOR, NULL, NULL);
	AppendMenu(Argand, MF_POPUP, (UINT_PTR)InputType, "Input Type");

	
	AppendMenu(Menu, MF_POPUP, (UINT_PTR) Argand, "Argand Diagrams");
}

void Window::Menus::setMenus(HWND hWnd) const {
	SetMenu(hWnd, Menu);
}

Window::Window(int width, int height, const char* name)
	:
	width(width), height(height), argand(hWnd, ArgandConfig::ABI)
{

	
		

	//Creates window
	//param 1 predefined class name from singleton
	//param 2 name passed through as constructor arg
	//param 3 window styles
	//param 4&5 window will "spawn" at the default location for a window
	//param 6&7 width and height of the window
	//param 8&9 ignore
	//param 10 instance found from predefined getinstance function in singleton
	//param 11 pointer to our window instance
	hWnd = CreateWindow(
		MainWindowClass::GetName(),name, WS_CAPTION|WS_MINIMIZEBOX|WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT, width, height, nullptr, nullptr,
		MainWindowClass::GetInstance(), this
		);

	
	config.fpDefault = &SetConfigDefault;
	
	config.fpDefault(&config);
	

	ShowWindow(hWnd, SW_SHOWDEFAULT);

	

}
//Pretty self explanitory lmao
Window::~Window() {
	//DeleteMenu(pmenu);
	DestroyWindow(hWnd);
}
void Window::SetTitle(const std::string& Title) {
	SetWindowText(hWnd, Title.c_str());
}


LRESULT WINAPI Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept {


	//wm_nccreatesent before WM_CREATE 
	//This means that in the lparam there will be a CREATESTRUCT and this will have a member called lpCreateParams
	//This contains the data of the "this" we put in the original create window function !!!!!!!!
	//So throught the create struct we get from NCCREATE we can acess the data given by the this
	//how we connect pointer to our class instance window to the winapi message handling mechanism
	if (msg == WM_NCCREATE) {
		//extract window class from creation data
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		//doing this static cast as the data in lpCreateparams is a Winddow* in disguise (coz we passed this in)
		Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);
		//set Winapi managed user data to store ptr to window class
		//We can set some userdata accociated with a particular window
		//we want to store a pointer to our window class on the winapi side
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
		// set messsage proc to normal non setup handler now setup is done
		//now we have installed ptr j window we want to use a different message proc
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::InvokeMemberFunc));

		return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

//This function is just an adapter Converts from win32api to my c++ code
LRESULT WINAPI Window::InvokeMemberFunc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept {
	//retreve pointer to window class
	Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	//Forward message to class handler
	return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
}

LRESULT WINAPI Window::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept{
	switch (msg) {
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	case WM_CREATE:
		menus.setMenus(hWnd);
		break;
	case WM_COMMAND:
		switch (wParam) {
		//Argand Diagram Shiz
		case A_BI:
			config.ArgandInputType = ArgandConfig::ABI;
			break;
		case MODARG:
			config.ArgandInputType = ArgandConfig::MOD_ARG;
			break;
		case EI:
			config.ArgandInputType = ArgandConfig::REI;
			break;
		case DRAW_ARGAND_DIAGRAM:
			if (argand.m_State.InputType == ArgandConfig::ABI) {
				MessageBox(nullptr, "swag", "Titties", MB_ICONWARNING);
			}
			break;
		}
	
	}
	argand.SetInputType(config.ArgandInputType);

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

std::optional<int> Window::ProcessMessage() {
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

void Window::ClearChildWindowBuffer() {
	while (std::size(childWindowBuff) >= 0) {
		childWindowBuff.pop_back();
	}
}

void Window::WarningBuffOverflow() {
	if (unsigned int i = std::size(childWindowBuff) > WindowBufflenMax) {
		while (i >= WindowBufflenMax) {
			childWindowBuff.pop_back();
		}
		MessageBox(nullptr, "Window Buffer Limit Reached\n\nToo Many Inputs Created", "Warning", MB_ICONWARNING);
	}
}