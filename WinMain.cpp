
#include"App.h"
#include"Logger.h"


int CALLBACK WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow) {
	
#ifndef NDEBUG
	Log* l = new Log;
	l->WipeFile();
	delete l;
#endif
	
	App myApp;

	
	myApp.Run();
	

	return 0;
}