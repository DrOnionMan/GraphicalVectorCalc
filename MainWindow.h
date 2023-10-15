#pragma once
#include<windows.h>
#include<optional>
#include<string>
#include<vector>

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

	class Menus {
	public:
		void setMenus(HWND hWnd)const ;
		Menus();
		~Menus();
	private:
		void CreateMainMenu();
		//AS Further
		void SetMatriciesTopic();
		void Set3DVectorsTopic();
		void SetArgandDiagramTopic();
		//General
		void HelpMenu();
	public:
		static constexpr int FILE_MENU = 69;
	public:
		HMENU Menu;
	private:
		std::vector<Menus> subMenus;
	};
public:
	Window(int width, int height, const char* name);
	~Window();
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
	void SetTitle(const std::string& Title);
	static std::optional<int> ProcessMessage();

	
private:
	static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	static LRESULT CALLBACK InvokeMemberFunc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	

private:
	Menus menus;
	int width;
	int height;
	HWND hWnd;
};