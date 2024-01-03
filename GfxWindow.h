#pragma once
#include<assert.h>
#include<windows.h>
#include"Keyboard.h"
#include"Mouse.h"
#include"Graphics.h"
#include<optional>
#include<string>
#include<memory>

class gfxWindow {
	
private:
	//singleton manages registration & cleanup of window class
	class gfxWindowClass {
	public:
		static const char* GetName() noexcept;
		static HINSTANCE GetInstance() noexcept;
	private:
		gfxWindowClass() noexcept;
		~gfxWindowClass();
		gfxWindowClass(const gfxWindowClass&) = delete;
		gfxWindowClass& operator = (const gfxWindowClass&) = delete;
		static constexpr const char* wndClassName = "Graphics engine gfxWindow";
		static gfxWindowClass wndClass;
		HINSTANCE hInst;
	};
public:
	gfxWindow(int width, int height, const char* name, bool* isalive, void* list);
	~gfxWindow();
	gfxWindow(const gfxWindow&) = delete;
	gfxWindow& operator=(const gfxWindow&) = delete;
	void SetTitle(const std::string& Title);
	static std::optional<int> ProcessMessage();
	void ShowgfxWindow();
	Graphics& GFX();
private:
	static LRESULT CALLBACK HandleMsgSetup_gfx(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	static LRESULT CALLBACK InvokeMemberFunc_gfx(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	void AdjustgfxWindowRect(LPARAM lp);
public:
	Keyboard kbd;
	Mouse mouse;
	HWND hWnd;
	bool* alive;
private:
	struct {
		float width;
		float height;
	}screenData;
	int width;
	int height;
	void* m_list;
	Graphics* pGfx = nullptr;
	//std::unique_ptr<Graphics> pGfx;
};


