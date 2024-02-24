#pragma once
#include<windows.h>


#define MAX_LEN 100


#define TOSTR(x){#x}

#define NEW OutputDebugString("\n")
#define MB(x) (MessageBox(nullptr, x, "Test", MB_ICONERROR))
#define ERR(Title, Msg) (MessageBox(nullptr, Msg, Title, MB_ICONERROR))
#define SIZE(x) ((sizeof(x)/ sizeof(x[0]) ))
#define OUTSTR(x) NEW; OutputDebugString(x)

#define INBOUND_INC(x, y, z) ( ( z >= x ) && ( z <= y ) ? 1 : 0)