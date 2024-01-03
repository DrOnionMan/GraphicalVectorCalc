#pragma once



#include"MainWindow.h"
#include"GfxWindow.h"



#include<chrono>


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