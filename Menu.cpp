#include"Menu.h"
#include<assert.h>


void Menus::setMenus() const {
	SetMenu(win, Menu);
}

void Menus::CreateMainMenu() const{
	SetArgandDiagramTopic();
	SetMatriciesTopic();
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

	


	AppendMenu(Argand, MF_STRING, GET_DIFF_FORM_ARGAND, "Format Converter");
	AppendMenu(Argand, MF_STRING, DRAW_ARGAND_DIAGRAM, "Argand Diagram Drawer");
	AppendMenu(Argand, MF_SEPARATOR, NULL, NULL);


	AppendMenu(Menu, MF_POPUP, (UINT_PTR)Argand, "Argand Diagrams");
}

void Menus::SetMatriciesTopic() const {
	HMENU Matricies = CreateMenu();

	AppendMenu(Menu, MF_STRING, DRAW_MATRICIES_SETUP, "Matricies");
}

