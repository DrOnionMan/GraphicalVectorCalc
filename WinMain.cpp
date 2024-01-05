
#include"App.h"
#include"Logger.h"


int CALLBACK WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow) {
	Log* l = new Log;
	l->WipeFile();
	delete l;

	
	App myApp;

	
	myApp.Run();
	

	return 0;
}