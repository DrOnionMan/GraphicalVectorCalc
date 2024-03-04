#include"Menu.h"
#include<assert.h>


void Menus::setMenus() const noexcept{
	//bind the menus created by class to the main window handle
	SetMenu(win, Menu);
}

void Menus::CreateMainMenu() const noexcept {
	//set the argand diagram topic menu
	SetArgandDiagramTopic();
	//set the matrix topic menu
	SetMatriciesTopic();
}

//empty class constructor
Menus::Menus() noexcept : win(NULL), Menu(CreateMenu()) {
	CreateMainMenu();
}

//constructor sets class member win to the window handle passed in
Menus::Menus(HWND hWnd) noexcept : win(hWnd), Menu(CreateMenu()) {
	CreateMainMenu();
}

Menus::~Menus() noexcept {
	//destroy the menus in class distructor
	DestroyMenu(Menu);
}

void Menus::SetArgandDiagramTopic() const noexcept {
	HMENU Argand = CreateMenu();
	//Create submenu argand

	//append the nececary menus ton the window
	AppendMenu(Argand, MF_STRING, GET_DIFF_FORM_ARGAND, "Format Converter");
	AppendMenu(Argand, MF_STRING, DRAW_ARGAND_DIAGRAM, "Argand Diagram Drawer");
	AppendMenu(Argand, MF_SEPARATOR, NULL, NULL);

	//This doesnt work without the UINT_PTR cast not sure why

	AppendMenu(Menu, MF_POPUP, (UINT_PTR)Argand, "Argand Diagrams");
}

void Menus::SetMatriciesTopic() const noexcept {
	
	//append matrix menu to Menu
	AppendMenu(Menu, MF_STRING, DRAW_MATRICIES_SETUP, "Matricies");
}

