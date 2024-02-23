#pragma once
#include<windows.h>
#include<unordered_map>


#ifndef NDEBUG
#define DXDEBUG

#define FUNC_ASSERT(x) (Debug(x, __LINE__, __FILE__))


#define ADDMAP(FResult) {(HRESULT)FResult, #FResult}

class DebugDX {
public:
	DebugDX();
	~DebugDX(){}

	void operator()(HRESULT FResult, int line, const char* file) const noexcept;
	std::unordered_map<HRESULT, std::string> map;
};



extern DebugDX Debug;

#endif


#ifdef NDEBUG

#define FUNC_ASSERT(x) (x)

#endif