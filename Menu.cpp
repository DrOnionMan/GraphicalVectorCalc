#include"Menu.h"
#include<assert.h>


void Menus::setMenus() const {
	SetMenu(win, Menu);
}

void Menus::CreateMainMenu() const{
	SetArgandDiagramTopic();
}




Menus::Menus() : win(NULL), Menu(CreateMenu()) {
	CreateMainMenu();
}

Menus::Menus(HWND hWnd) : win(hWnd), Menu(CreateMenu()) {
	CreateMainMenu();
}

Menus::~Menus() {
	DestroyMenu(Menu);
}

void Menus::SetArgandDiagramTopic() const{
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


	AppendMenu(Menu, MF_POPUP, (UINT_PTR)Argand, "Argand Diagrams");
}

