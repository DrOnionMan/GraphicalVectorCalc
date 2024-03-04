#include"App.h"
#include"Logger.h"

int CALLBACK WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow) {
	//clear my logfile
#ifndef NDEBUG
	Log* l = new Log;
	l->WipeFile();
	delete l;
#endif
	//make the app
	App myApp;
	//ill let you guess
	myApp.Run();
	return 0;
}