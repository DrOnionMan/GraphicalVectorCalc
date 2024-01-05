#include"MainWindow.h"
#include"MenuMacroDef.h"
#include"MacroUtils.h"
#include"StringUtils.h"
#include"Logger.h"

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
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
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


#include"Logger.h"

Window::Window(int width, int height, const char* name)
	:
	width(width), height(height), hWnd(CreateWindow(
		MainWindowClass::GetName(), name, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT, width, height, nullptr, nullptr,
		MainWindowClass::GetInstance(), this
	)), pMenus(nullptr), GraphicsIsAlive(false)
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
	
	
	
	config.fpDefault = &SetConfigDefault;
	
	config.fpDefault(&config);
	
	parser.setParserConfig(&config);
	

	ShowWindow(hWnd, SW_SHOWDEFAULT);

}

//Pretty self explanitory lmao
Window::~Window() {
	delete pMenus;
	
	DestroyWindow(hWnd);
}



void Window::SetTitle(const std::string& Title) {
	SetWindowText(hWnd, Title.c_str());
}


LRESULT WINAPI Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept {


	
	if (msg == WM_NCCREATE) {
		
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		
		Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);

		pWnd->pMenus = new Menus(hWnd);

		//set Winapi managed user data to store ptr to window class
		//We can set some userdata accociated with a particular window
		//we want to store a pointer to our window class on the winapi side
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));

		// set messsage proc to normal non setup handler now setup is done
		//now we have installed ptr j window we want to use a different message proc

		SetWindowLongPtr(hWnd, GWLP_WNDPROC , reinterpret_cast<LONG_PTR>(&Window::InvokeMemberFunc));

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
	{
		pMenus->setMenus();
		
		break;
	}
	case WM_COMMAND:
		PollArgandEvents(wParam, lParam);
		break;
	
	}
	

	

	return DefWindowProc(hWnd, msg, wParam, lParam);
}



typedef uint64_t ui64;
void Window::PollArgandEvents(WPARAM wParam, LPARAM lParam) {
	

	
	switch (wParam) {
		//Argand Diagram Shiz
	case ADD_COMPLEX_GEOMETRY:
		if (!BufferFull()) {
			Argand::AddGeometryBox(hWnd, &childWindowBuff);
		}
		else {
			WarningBuffOverflow();
		}
		break;
	case DRAW_ARGAND_DIAGRAM:
		ClearChildWindowBuffer();
		if (!BufferFull()) {
			Argand::SetupDraw(hWnd, &childWindowBuff);
		}
		else {
			WarningBuffOverflow();
		}

		break;
	case GET_DIFF_FORM_ARGAND:
		ClearChildWindowBuffer();
		if (!BufferFull()){ 
			Argand::SetupConverter(hWnd, &childWindowBuff);
		}
		else {
			WarningBuffOverflow();
		}
		break;
	case CONVERT:
		if (!BufferFull()) { 
			parser.setParserText(Argand::GetStringFromEdit(GetChild(first, &childWindowBuff, EDIT)), NOPROC);
			complex c = parser.ParsetoComplex();
			Argand::DisplayConverterResult(c, &childWindowBuff, hWnd);
			return;
		}
		else { 
			WarningBuffOverflow();
		}
		break;
	case BN_CLICKED:
	{

		for (int i = 0; i < BufflenMax; i++) {
			if (childWindowBuff.at(i).type == EDIT) {
				if (childWindowBuff.at(i).associate->id == (HWND)lParam) {
					DeleteEdit(&childWindowBuff, i);
					//TODO
					//RearrangeEdits();
					//Used emplace edits instead @Arg Di menu.cpp
					return;
				}
			}
		}
	}break;
	case DRAW_ARGAND:
	{
		GeomData gData = { 0 };
		//DeleteList(head);
		for (children c : childWindowBuff) {
			if (c.type == EDIT) {
				gData = parser.ProcExprStrToGeom(Argand::GetStringFromEdit(&c));
				if (!gData) {
					DeleteList(head);
					return;
				}
				head += gData;
				
				
			}
		}

		quick_sort(head, last_node(head));
		
		gwin = new gfxWindow(900, 600, "Graphics Window", &GraphicsIsAlive, head);
		GraphicsIsAlive = true;
		
		
	}break;
	}
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
	if (childWindowBuff.size() == 0) return;
	for (int i = childWindowBuff.size(); i > 0; i--) {
		if (childWindowBuff[i-1].associate) {
			DestroyWindow(childWindowBuff[i-1].associate->id);
			delete childWindowBuff[i - 1].associate;
		}
		DestroyWindow(childWindowBuff[i-1].id);
		childWindowBuff.pop_back();
	}
}

void Window::TrimBuffer() {
	int i = childWindowBuff.size() - 1;
	while (childWindowBuff.size() > BufflenMax) {
		MB("TrimBuffer called");
		DestroyWindow(childWindowBuff[i].id);
		childWindowBuff.pop_back();
		i--;
	}

}

void Window::WarningBuffOverflow() {
	if (BufferFull()) {
		MessageBox(nullptr, "Window Buffer Limit Reached\n\nToo Many Inputs Created", "WARNING", MB_ICONWARNING);
		TrimBuffer();
	}
}

bool Window::BufferFull() const{
	return childWindowBuff.size() < BufflenMax ? false : true;
}

