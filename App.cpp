#include"App.h"


App::App() : wnd(800, 600, "MainWnd") {}


int App::Run() {
	while (true) {
		if (const auto ecode = Window::ProcessMessage()) {
			return *ecode;
		}

	}
}