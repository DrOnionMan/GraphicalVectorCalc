#include"DebugDX.h"
#include<string>
#include<sstream>
#include<thread>
#include<chrono>



#ifdef DXDEBUG


DebugDX Debug;

DebugDX::DebugDX() : map({
	//fill map with all the common HRESULT falures
	//general HRESULTs
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
	ADDMAP(E_UNEXPECTED),
	//DXGI specific
	ADDMAP(DXGI_ERROR_DEVICE_REMOVED),
	ADDMAP(DXGI_ERROR_DEVICE_RESET),
	ADDMAP(DXGI_ERROR_UNSUPPORTED),
	ADDMAP(DXGI_ERROR_NOT_FOUND),
	ADDMAP(DXGI_ERROR_INVALID_CALL),
	ADDMAP(DXGI_ERROR_WAS_STILL_DRAWING)

	}) {
	
}

#define THROWHRERROR(hres, hres_str, line, file)\
do{\
	std::ostringstream Title;\
	Title << "DXGI ASSERTION FALURE";\
	std::ostringstream ErrorMsg;\
	ErrorMsg << "Error Code: { " << hres_str << " }\n" \
	<< "Error Occurred on Line: "  << line << "\nIn File: " << file << "\n\n" << "Error: ";\
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
	switch((HRESULT)hres){\
		case DXGI_ERROR_DEVICE_REMOVED:\
			ErrorMsg << "GPU Device removed or Driver Falure.";break;\
		case DXGI_ERROR_DEVICE_RESET:\
			ErrorMsg << "GPU encountered error, Device reset.";break;\
		case DXGI_ERROR_UNSUPPORTED:\
			ErrorMsg << "Functionallity not supported.";break;\
		case DXGI_ERROR_NOT_FOUND:\
			ErrorMsg << "Requested item not found.";break;\
		case DXGI_ERROR_INVALID_CALL:\
			ErrorMsg << "method call invalid.";break;\
		case DXGI_ERROR_WAS_STILL_DRAWING:\
			ErrorMsg << "previous operation still ongoing.";break;\
	}\
	MessageBox(NULL,ErrorMsg.str().c_str(),Title.str().c_str(), MB_ICONERROR);\
}while(0)\


void DebugDX::operator()(HRESULT Fresult, int line , const char* file) const noexcept{
	//get entry
	const auto entry = map.find(Fresult);
	if (entry != map.end()) {
		switch (Fresult) {
		case S_OK:
			//ignore if it is ok
			return;
		default:
			//throw error if its not ok
			THROWHRERROR(entry->first, entry->second, line, file);
			break;
		}
	}
	//if error terminate program
	exit(-1);
}


#endif
