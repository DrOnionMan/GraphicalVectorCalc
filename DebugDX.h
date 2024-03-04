#pragma once
#include<windows.h>
#include<unordered_map>


#ifndef NDEBUG
#define DXDEBUG
//uses the Debug variable & line & file macros included
#define FUNC_ASSERT(x) (Debug(x, __LINE__, __FILE__))
//adds the Fresult to map seen in .cpp file
#define ADDMAP(FResult) {(HRESULT)FResult, #FResult}
//simple class
class DebugDX {
public:
	DebugDX();
	~DebugDX(){}
	void operator()(HRESULT FResult, int line, const char* file) const noexcept;
private:
	std::unordered_map<HRESULT, std::string> map;
};
//extern for obvs reasons
//not for use in this unit
extern DebugDX Debug;
#else
#define FUNC_ASSERT(x) (x)
#endif