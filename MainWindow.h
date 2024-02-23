#pragma once
#include<windows.h>
#include<optional>
#include<sstream>
#include<vector>
#include"Argand_Diagram_Menu.h"
#include"children.h"
#include"Menu.h"
#include"Parser.h"
#include"GfxWindow.h"
#include"LinkedList.h"



//Handles Window Creation & distruction
class Window {


private:

	//singleton manages registration & cleanup of window class
	class MainWindowClass {
	public:
		static const char* GetName() noexcept;
		static HINSTANCE GetInstance() noexcept;
	private:
		MainWindowClass() noexcept;
		~MainWindowClass();
		MainWindowClass(const MainWindowClass&) = delete;
		MainWindowClass& operator = (const MainWindowClass&) = delete;
		static constexpr const char* wndClassName = "blank window";
		static MainWindowClass wndClass;
		HINSTANCE hInst;
	};

	

public:
	Window(int width, int height, const char* name);
	~Window();
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
	void SetTitle(const std::string& Title);
	static std::optional<int> ProcessMessage();
	bool DoCreateGraphics();
	
	
private:
	static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	static LRESULT CALLBACK InvokeMemberFunc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	
	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	void PollMyEvents(WPARAM wParam, LPARAM lParam);
	bool BufferFull()const;
	void ClearChildWindowBuffer();
	void WarningBuffOverflow();
	void TrimBuffer();
	


private:
	const Menus* pMenus;
	int width;
	int height;
	Parser parser;
public:
	node* head = NULL;
	bool GraphicsIsAlive;
	gfxWindow* gwin;
	HWND hWnd;
	static constexpr inline unsigned int BufflenMax = 7u;
	std::vector<children> childWindowBuff;
};