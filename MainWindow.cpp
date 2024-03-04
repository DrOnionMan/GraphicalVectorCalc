#include"MainWindow.h"
#include"MenuMacroDef.h"
#include"MacroUtils.h"
#include"StringUtils.h"
#include"Logger.h"
#include<ranges>


//singleton to handle window class cleanup and destruction
Window::MainWindowClass Window::MainWindowClass::wndClass;

Window::MainWindowClass::MainWindowClass() noexcept
	: hInst(GetModuleHandle(nullptr)) {
	WNDCLASSEX wc = { 0 };
	//register window class
	wc.cbSize = sizeof(wc);
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hCursor = nullptr;
	wc.hIcon = nullptr;
	wc.lpszMenuName = nullptr;
	//Clears window and draws every frame
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wc.hInstance = GetInstance();
	wc.lpszClassName = GetName();
	wc.style = CS_OWNDC;
	wc.hIconSm = nullptr;
	//class member in window
	wc.lpfnWndProc = HandleMsgSetup;
	RegisterClassEx(&wc);
}
Window::MainWindowClass::~MainWindowClass() {
	//Unregister o class distruction
	UnregisterClass(wndClassName, GetInstance());
}
const char* Window::MainWindowClass::GetName() noexcept {
	//getter
	return wndClassName;
}
HINSTANCE Window::MainWindowClass::GetInstance() noexcept {
	//getter
	return wndClass.hInst;
}


#include"Logger.h"

Window::Window(int width, int height, const char* name) noexcept
	:
	//initialising class members, all pointers initialised to nullptr
	width(width), height(height), hWnd(CreateWindow(
		MainWindowClass::GetName(), name, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT, width, height, nullptr, nullptr,
		MainWindowClass::GetInstance(), this
	)), pMenus(nullptr), 
	//boolian to say if graphics window should be rendered
	GraphicsIsAlive(false),
	//pointer to graphics window
	gwin(nullptr)
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
	

	ShowWindow(hWnd, SW_SHOWDEFAULT);

}

//Pretty self explanitory lmao
Window::~Window() {
	delete pMenus;
	
	DestroyWindow(hWnd);
}






//Legacy function not usegul anymore
void Window::SetTitle(const std::string& Title) noexcept {
	SetWindowText(hWnd, Title.c_str());
}


LRESULT WINAPI Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept {
	//check for nccreate message
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
		//return class member
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


//class message handler below
LRESULT WINAPI Window::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept{
	
	
	switch (msg) {
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	case WM_CREATE:
	{
		//on create set menus with menus class pointer.
		pMenus->setMenus();
		
		break;
	}
	case WM_COMMAND:
		PollMyEvents(wParam, lParam);
		break;
	
	}
	

	

	return DefWindowProc(hWnd, msg, wParam, lParam);
}




static bool lessthan(node* left, node* right) {
	if (left->data.gType < right->data.gType) {
		return true;
	}

	return false;
}



void Window::MakeKey(bool is3D) noexcept{
	if (!is3D) {
		std::vector<std::string> cols = {"Pink", "Yellow", "Blue", "Orange", "Red"};
		std::ostringstream oss;
		oss << "Key: \n";
		int i = 0;
		for (node* cn = head; cn != NULL; cn = cn->next) {
			oss << cn->data.name << " -> " << cols.at(i) << "\n\n";
			i++;
		}

		

		children temp = { 0 };
		temp.associate = nullptr;
		temp.id = CreateWindow("Static",
			oss.str().c_str(),
			WS_VISIBLE | WS_CHILD | WS_BORDER,
			450, 290, 300,250,
			this->hWnd, NULL, NULL, NULL
		);

		temp.type = STATIC;
		childWindowBuff.push_back(temp);
	}

	else {

		std::vector<std::string> cols = { "Pink", "Orange", "Light Blue"};
		std::ostringstream oss;
		oss << "Key: \n";
		int i = 0;

		
		
		
		oss << "Colour of axis +ve -> -ve\n"
			<< "X-Axis = Red -> white\n" <<
			"Y-Axis = Green -> white\n" <<
			"Z-Axis = Blue -> white\n\n";

		for (node* cn = head; cn != NULL; cn = cn->next) {
			oss << "Plane :\n" 
				<< cn->data.data.mat.a << "x + " << cn->data.data.mat.b << "y + "<<
				cn->data.data.mat.c << "z = " << cn->data.data.mat.d << " -> " << cols.at(i) << "\n\n";
			
			i++;
		}

		

		children temp = { 0 };
		temp.associate = nullptr;
		temp.id = CreateWindow("Static",
			oss.str().c_str(),
			WS_VISIBLE | WS_CHILD | WS_BORDER,
			450, 290, 300, 250,
			this->hWnd, NULL, NULL, NULL
		);

		temp.type = STATIC;
		childWindowBuff.push_back(temp);
	}
}

#include"DebugDX.h"

typedef uint64_t ui64;
void Window::PollMyEvents(WPARAM wParam, LPARAM lParam) noexcept {
	
	
	switch (wParam) {
		//Argand Diagram Shiz
	case ADD_COMPLEX_GEOMETRY:
		if (!BufferFull()) {
			//adds a text box to window using emplace edit function
			Argand::AddGeometryBox(hWnd, &childWindowBuff);
		}
		else {
			WarningBuffOverflow();
		}
		break;
	case DRAW_ARGAND_DIAGRAM:
		//Clear residual stuff from screen
		ClearChildWindowBuffer();
		//check buffer is not full
		if (!BufferFull()) {
			//Call setup draw function passing in my window handle 
			//and a pointer to my std::vector of child windows as defined in Header file
			Argand::SetupDraw(hWnd, &childWindowBuff);
		}
		else {
			//Called when buffer is full
			WarningBuffOverflow();
		}

		break;
	case GET_DIFF_FORM_ARGAND:
		ClearChildWindowBuffer();
		if (!BufferFull()){ 
			//Sets up the converter pretty self explanitory
			Argand::SetupConverter(hWnd, &childWindowBuff);
		}
		else {
			WarningBuffOverflow();
		}
		break;
	case CONVERT:
		if (!BufferFull()) { 
			//set the prser text to the edit on the window
			//NOPROC flag is a legacy thing and a result of poor planning on my part, ignore it and any reference
			//to procstring function found in class parser.
			parser.setParserText(Argand::GetStringFromEdit(GetChild(first, &childWindowBuff, EDIT)), NOPROC);
			complex c = parser.ParsetoComplex();
			//display result
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
		//if win is running dont let the user click draw again, obviously...
		if (GraphicsIsAlive) {
			return;
		}
		//assign empty geomdata
		GeomData gData = { 0 };
		//foreach 
		for (children c : childWindowBuff) {
			//if window is a textbox
			if (c.type == EDIT) {
				//retrieve geometry data from the user input
				gData = parser.ProcExprStrToGeom(Argand::GetStringFromEdit(&c));
				//OVERLOAD
				if (!gData || gData.gType == MAT) {
					//if the input is invalid delete the linked list to prevent memory leak.
					DeleteList(head);
					//Parser gives more detail on errors that occured
					return;
				}
				for (node* currentnode = head; currentnode != NULL; currentnode = currentnode->next) {
					//check if 2 Geoms have the same name
					if (currentnode->data.name == gData.name) {
						// if they do delete list & goto error handler
						DeleteList(head);
						//goto to handle errors :) Cursed IK
						//got flamed for doing this in r/C++
						goto IN_ERROR;
					}
				}
				//OVERLOAD - add the gData to my linked list.
				head += gData;
			}
		}
		//Is my linked list still NULL??
		if (!head) {
			goto IN_ERROR;
		}
		//sort them out!!!!!!!!!!!
		//puts them in draw order based on enum.
		quicksort_c(head, last_node(head), lessthan);

		//makes the key, not 3d hence false
		MakeKey(false);
		//make that window :)))
		gwin = new gfxWindow(900, 600, "Graphics Window", { &GraphicsIsAlive, false }, head);
		//Yes i want to run my graphics!!!!!!!
		GraphicsIsAlive = true;
		return;
	IN_ERROR:
		ERR("Input Error", "Invalid geometry input in entry field(s)\nBreaking...");
	}break;
	case DRAW_MATRICIES_SETUP: {
		ClearChildWindowBuffer();
		if(!BufferFull()) {
			Mats::SetupMats(hWnd, &childWindowBuff);
		}
		else {
			WarningBuffOverflow();
		}
	}break;
	case M_TRANSMIT: {
		if (GraphicsIsAlive) {
			return;
		}
		GeomData gData = { 0 };
		//foreach
		for (children c : childWindowBuff) {
			if (c.type == EDIT) {
				gData = parser.ProcExprStrToGeom(Argand::GetStringFromEdit(&c));
				if (!gData || gData.gType != GeomType::MAT) {
					DeleteList(head);
					return;
				}
				head += gData;
			}
		}
		
		if (!head) {
			ERR("Argument Error", "No geometry arguments provided.");
			return;
		}
		//makekey true as is 3d
		MakeKey(true);
		//create window
		gwin = new gfxWindow(900, 600, "Graphics Window", { &GraphicsIsAlive, true }, head);
		GraphicsIsAlive = true;
		
	}break;
	}
}

std::optional<int> Window::ProcessMessage() noexcept {
	MSG msg;
	//peek so render loop not interrupted, not useful on this window but its not a detriment either.
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
		if (msg.message == WM_QUIT) {
			return msg.wParam;
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return {};
}

void Window::ClearChildWindowBuffer() noexcept {
	if (childWindowBuff.size() == 0) return;
	//love a little bit of the C++ STL
	//trivial find and delete pretty self explanitory
	for (children& child : childWindowBuff | std::ranges::views::reverse) {
		if (child.associate != nullptr) {
			DestroyWindow(child.associate->id);
			delete child.associate;
		}
		DestroyWindow(child.id);
	}
	childWindowBuff = {};
}

void Window::TrimBuffer() noexcept {
	int i = childWindowBuff.size() - 1;
	while (childWindowBuff.size() > BufflenMax) {
		//MB("TrimBuffer called");
		DestroyWindow(childWindowBuff[i].id);
		childWindowBuff.pop_back();
		i--;
	}

}

void Window::WarningBuffOverflow() noexcept {
	if (BufferFull()) {
		//warn the user if they add too many text boxes!
		MessageBox(nullptr, "Window Buffer Limit Reached\n\nToo Many Inputs Created", "WARNING", MB_ICONWARNING);
		TrimBuffer();
	}
}

bool Window::BufferFull() const noexcept {
	int count = 0;
	for (const auto& c : childWindowBuff) {
		if (c.type == EDIT) {
			count++;
		}
	}
	if (count == 5) {
		return true;
	}
	return childWindowBuff.size() < BufflenMax ? false : true;
}

