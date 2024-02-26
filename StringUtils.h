#pragma once
#include"MacroUtils.h"
#include<cstdarg>
#include<string>
#include<string.h>
#include<sstream>

#define STRUtils

#define TOUPPER(x) ( x -= 0x20 )


void strcpy(std::string& src, int length, char* dest);


void popSubstr(char* source, int len, char* destination);

char* allocstr(int size);


bool isnum_real(char* text);

bool isnum_complex(char* text);

char* FindUnexpectedToken(char* buff, bool complex);

int findIndex(char* buff, char ch);


#ifndef NDEBUG

static std::ostringstream oss;
template<typename T, typename...Args>
void PrintNums(T first, Args...args) { 
	oss << first;
	if constexpr (sizeof...(args) > 0) {
		oss << " , ";
		PrintNums(args...);
	}
	else {
		MB(oss.str().c_str());
		oss = {};
	}
}

#endif


