#include"Menu.h"

MKMenu* InitialiseMenu() {
	MKMenu* menu = (MKMenu*)malloc(sizeof(MKMenu));
	menu->menu = CreateMenu();
	menu->fpSetMenu = SetMenu;
	return menu;
}

void DeleteMenu(MKMenu* menu) {
	free((void*)menu);
}

void AddMenu(MKMenu* menu) {
	AppendMenuA(menu->menu, MF_STRING, 69, "Menu!!!!!");
}


void AddSubMenu(MKMenu menu) {

}