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

//wrap in ifndef as souldnt be included in realease
#ifndef NDEBUG
//static to stop annoying multiple symbol error
static std::ostringstream oss;
template<typename T, typename...Args>
//so happy variatic templetes exist :((
void PrintNums(T first, Args...args) { 
	//pipe in first arg
	oss << first;
	//allow for tasty compile time optimisations with the constexpr
	if constexpr (sizeof...(args) > 0) {
		//is there more args????????????
		oss << " , ";
		//reecursively print them
		PrintNums(args...);
	}
	else {
		//Output a message box with the contents of the input
		MB(oss.str().c_str());
		//clear the string
		oss = {};
	}
}
#else
//if we are in release just ignore all calls to this function
#define PrintNums(x) {}
#endif


