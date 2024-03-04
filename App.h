#pragma once

#include"MainWindow.h"
#include"GfxWindow.h"

#include<chrono>

//just was used for measuring ms per frame
//no longer used
class Timer {
public:
	Timer();
	float Mark();
	float Peek() const;
private:
	std::chrono::steady_clock::time_point last;
};



class App {
public:
	App();
	int Run();
	void DoFrame();
private:
	Window wnd;
	Timer Clock;
};