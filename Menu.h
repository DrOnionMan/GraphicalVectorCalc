#pragma once
#include<windows.h>
#include"MenuMacroDef.h"


class Menus {
	friend class Window;
public:
	void setMenus() const noexcept;
	Menus() noexcept;
	Menus(HWND hWnd) noexcept;
	Menus(const Menus&) = delete;
	Menus& operator=(const Menus&) = delete;
	~Menus() noexcept;

private:
	void CreateMainMenu() const noexcept;
	//AS Further
	void SetMatriciesTopic() const noexcept;
	void SetArgandDiagramTopic() const noexcept;
	
public:
	const HMENU Menu;
private:
	const HWND win;
};