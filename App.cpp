#include"App.h"
#include"MacroUtils.h"
#include"MenuMacroDef.h"



Timer::Timer() {
	last = std::chrono::steady_clock::now();
}

//Gives u time elapsed since the last time u called the biach
float Timer::Mark() {
	const auto old = last;
	last = std::chrono::steady_clock::now();
	const std::chrono::duration<float> frameTime = last - old;
	return frameTime.count();
}

//time elapsed since mark without reseting mark point :))))))))
float Timer::Peek() const {
	return std::chrono::duration<float>(std::chrono::steady_clock::now() - last).count();
}




App::App() : wnd(800, 600, "MainWnd") {}

void App::DoFrame() {
	

	wnd.gwin->GFX().ClearBuffer(0.0f, 0.0f, 0.0f);
	if (wnd.gwin->kbd.KeyIsPressed('A')) {
		wnd.gwin->GFX().PrintVp();
	}

	//wnd.gwin->GFX().DrawTriangle(Clock.Peek(),wnd.gwin->mouse.GetPosX() / 450.0f - 1.0f, -wnd.gwin->mouse.GetPosY() / 300.0f + 1.0f, L"PixelShader.cso");
	wnd.gwin->GFX().DrawAxis2D();
	wnd.gwin->GFX().EndFrame();
}

int App::Run() {
	while (true) {
		
		if (const auto ecode = Window::ProcessMessage()) {
			return *ecode;
		}
		if (wnd.GraphicsIsAlive) {
			DoFrame();
		}
		if (!wnd.GraphicsIsAlive && wnd.head != NULL) {
			wnd.head = NULL;
		}
	}
}