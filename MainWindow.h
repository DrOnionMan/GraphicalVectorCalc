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
	Window(int width, int height, const char* name) noexcept;
	~Window() noexcept;
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
	void SetTitle(const std::string& Title) noexcept;
	static std::optional<int> ProcessMessage() noexcept;
	
	
	
private:
	static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	static LRESULT CALLBACK InvokeMemberFunc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	
	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	void PollMyEvents(WPARAM wParam, LPARAM lParam) noexcept;
	bool BufferFull() const noexcept;
	void ClearChildWindowBuffer() noexcept;
	void WarningBuffOverflow() noexcept;
	void TrimBuffer() noexcept;

	void MakeKey(bool is3D) noexcept;
	


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
	static constexpr inline unsigned int BufflenMax = 8u;
	std::vector<children> childWindowBuff;
};