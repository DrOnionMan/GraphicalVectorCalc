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

#ifdef _DEBUG
template<typename T>
void PrintNums(int count...) {
	std::ostringstream oss;
	va_list ptr;
	va_start(ptr, count);

	for (int i = 0; i < count; i++) {
		oss << va_arg(ptr, T) << " , ";
	}
	va_end(ptr);

	std::string s = oss.str();


	const char* txt = s.c_str();


	MB(txt);
}
#endif


void DeleteBuffers(int count ...);
