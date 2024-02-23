#include"DebugDX.h"
#include<string>
#include<sstream>
#include<thread>
#include<chrono>



#ifdef DXDEBUG


DebugDX Debug;

DebugDX::DebugDX() : map({
	ADDMAP(S_OK),
	ADDMAP(E_ABORT),
	ADDMAP(E_ACCESSDENIED),
	ADDMAP(E_FAIL),
	ADDMAP(E_HANDLE),
	ADDMAP(E_INVALIDARG),
	ADDMAP(E_NOINTERFACE),
	ADDMAP(E_NOTIMPL),
	ADDMAP(E_OUTOFMEMORY),
	ADDMAP(E_POINTER),
	ADDMAP(E_UNEXPECTED)}) {
	
}

#define THROWHRERROR(hres, hres_str, line, file)\
do{\
	std::ostringstream Title;\
	Title << "Error Code: " << hres_str;\
	std::ostringstream ErrorMsg;\
	ErrorMsg << "Error Occurred on Line " << line << " In File " << file << "\n\n" << "Error: ";\
	switch((HRESULT)hres){\
		case E_ABORT:\
			ErrorMsg << "Operation has aborted no additional info."; break;\
		case E_ACCESSDENIED:\
			ErrorMsg << "General Access denied error has occured"; break;\
		case E_FAIL:\
			ErrorMsg << "Unexpected falure.";break;\
		case E_HANDLE:\
			ErrorMsg << "Handle is not valid.";break;\
		case E_INVALIDARG:\
			ErrorMsg << "One or more arguments are not valid.";break;\
		case E_NOINTERFACE:\
			ErrorMsg << "No sutch interface is supported.";break;\
		case E_NOTIMPL:\
			ErrorMsg << "Not implemented.";break;\
		case E_OUTOFMEMORY:\
			ErrorMsg << "Failed to allocate necessary memory.";break;\
		case E_POINTER:\
			ErrorMsg << "Pointer that is not valid.";break;\
		case E_UNEXPECTED:\
			ErrorMsg << "Unexpected failure.";break;\
	}\
	MessageBox(NULL,ErrorMsg.str().c_str(),Title.str().c_str(), MB_ICONERROR);\
}while(0)\


void DebugDX::operator()(HRESULT Fresult, int line , const char* file) const noexcept{


	const auto entry = map.find(Fresult);
	if (entry != map.end()) {
		switch (Fresult) {
		case S_OK:
			return;
		default:
			THROWHRERROR(entry->first, entry->second, line, file);
			break;
		}
	}
	


	
	exit(-1);
}




#endif
