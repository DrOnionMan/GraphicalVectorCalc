#include"StringUtils.h"
#include<stdlib.h>
#include"MacroUtils.h"
#include"MenuMacroDef.h"


void popSubstr(char* source, int len, char* destination) {
	for (int i = 0; i < len; i++) {
		destination[i] = source[i];
	}
}

char* allocstr(int size) {
	size++;
	char* x = (char*)malloc(sizeof(char) * size);
	if (!x) {
		MB("ALLOC ERROR");
		return nullptr;
	}

	memset(x, '\0',size);

	return x;
}




bool isnum_real(char* text) {
	for (char* p = text; *p != '\0'; p++) {
		if (*p == '£' || *p=='/' || *p == '.') {
			continue;
		}
		else if(INBOUND_INC('0', '9', *p)) {
			continue;
		}
		else {
			return false;
		}
	}
	return true;
}
bool isnum_complex(char* text) {
	for (char* p = text; *p != '\0'; p++) {
		if (*p == '£' || *p == '/' || *p == '.' || *p == 'I') {
			continue;
		}
		else if(INBOUND_INC('0', '9', *p)) {
			continue;
		}
		else {
			return false;
		}
	}

	return true;
}


char* FindUnexpectedToken(char* buff, bool complex) {
	char* buffer = allocstr(MAX_LEN);
	int cursor = 0;
	int count = 0;
	int icount = 0;

	for (char* p = buff; *p != '\0'; p++) {
		if (*p == '.' && count < 1) {
			count++;
			continue;
		}
		else if (*p == 'I' && icount < 1 && complex) {
			icount++;
			continue;
		}
		if (INBOUND_INC('0', '9', *p)) {
			continue;
		}
		else {
			if (cursor != 0) {
				cursor++;
			}
			buffer[cursor]   = '\"';
			buffer[++cursor] = *p;
			buffer[++cursor] = '\"';
			buffer[++cursor] = ' ';
			buffer[++cursor] = ',';
			buffer[++cursor] = ' ';
			
		}
	}

	return buffer;
}

int findIndex(char* buff, char ch, int count) {
	int charCount = 0;
	for (char* p = buff; *p != '\0'; p++) {
		if (*p == ch && count == charCount) {
			return (p - buff);
		}
		if (*p == ch && count != charCount) {
			charCount++;
		}
	}
	return -1;
}

bool checkStringIsValid(char* text, bool isComplex) {
	std::string str = text;
	int len = str.length();
	if (isComplex) {

	}
	else {
		//this is a count for the character . there should not be more than 1 per string
		int Uniquecount = 0;
		
		for (char c : str) {
			if (Uniquecount > 1) {
				ERR("INPUT INVALID", "The ");
				return false;
			}
			switch (c) {
			case '.':
				Uniquecount++;
				break;
			}
		}
	}
}




void strcpy(std::string& src, int length, char* dest) {
	if (!dest) {
		ERR("NULLPTR ERROR", "Function strcpy was passed a nullptr");
		return;
	}
	for (int i = 0; i < length; i++) {
		dest[i] = src[i];
	}
}