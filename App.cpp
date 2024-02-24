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

int i = 0;

void App::DoFrame() {
	

	wnd.gwin->GFX().ClearBuffer(0.0f, 0.0f, 0.0f);
	static bool wasPressed = false;
	
	if (wnd.gwin->GFX()._Dim) {
		if (wnd.gwin->kbd.KeyIsPressed('A')) {
			wnd.gwin->GFX().Arg_H += 0.1f;
		}
		if (wnd.gwin->kbd.KeyIsPressed('D')) {
			wnd.gwin->GFX().Arg_H -= 0.1f;
		}
		if (wnd.gwin->kbd.KeyIsPressed('W')) {
			wnd.gwin->GFX().Arg_V += 0.1f;
		}
		if (wnd.gwin->kbd.KeyIsPressed('S')) {
			wnd.gwin->GFX().Arg_V -= 0.1f;
		}

		
		if (wnd.gwin->kbd.KeyIsPressed('O') && !wasPressed) {
			if (wnd.gwin->GFX().SF > 0u) {
				wasPressed = true;
				std::ostringstream oss;
				oss << wnd.gwin->GFX().SF << "\n";
				wnd.gwin->GFX().SF -= 1u;
				OutputDebugString(oss.str().c_str());
			}
		}

		if (wnd.gwin->kbd.KeyIsPressed('I') && !wasPressed) {
			if (wnd.gwin->GFX().SF < 11u) {
				wasPressed = true;
				std::ostringstream oss;
				oss << wnd.gwin->GFX().SF << "\n";
				wnd.gwin->GFX().SF += 1u;
				OutputDebugString(oss.str().c_str());
			}
		}
		
		
		//ensure it is within the near and far viewing plane
		const auto& a = wnd.gwin->mouse.Read();
		if (a.GetType() == Mouse::Event::Type::WheelUp) {
			if(wnd.gwin->GFX().DTS > -8.0f)
			wnd.gwin->GFX().DTS -= 0.25f;
		}
		if (a.GetType() == Mouse::Event::Type::WheelDown) {
			if (wnd.gwin->GFX().DTS < -1.5f)
			wnd.gwin->GFX().DTS += 0.25f;
		}
		
	}
	
	if (i % 35 == 0) {
		wasPressed = false;
		//MB("happened");
	}


	i++;
	wnd.gwin->GFX().Render();
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