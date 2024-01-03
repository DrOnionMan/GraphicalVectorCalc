#pragma once
#include<windows.h>
#include"MenuMacroDef.h"


class Menus {
	friend class Window;
public:
	void setMenus() const;
	Menus();
	Menus(HWND hWnd);
	Menus(const Menus&) = delete;
	Menus& operator=(const Menus&) = delete;
	~Menus();
private:
	void CreateMainMenu() const;
	//AS Further
	void SetMatriciesTopic()const;
	void Set3DVectorsTopic()const;
	void SetArgandDiagramTopic() const;
	//General
	void HelpMenu();

public:
	const HMENU Menu;
private:
	const HWND win;
};