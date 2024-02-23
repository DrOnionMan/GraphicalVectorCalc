#pragma once
#include<string>
#include"MKMath.h"
#include"GeometryData.h"

#define PROC 1u
#define NOPROC 0u
#define NOSAN 2u



class Parser {


	
private:
	void ProcString();

public:
	Parser();
	Parser(const Parser&) = delete;
	Parser& operator=(const Parser&) = delete;
	void setParserText(char* text, unsigned int flag);
	~Parser();
	complex ParsetoComplex();

	GeomData ProcExprStrToGeom(char* txt);
private:
	bool StringIsValid(char* text, bool isComplex);
	void SanitiseInput();

	std::pair<int, int> FindToken(const char* token);
	char* GetToken(int start, int end);


	GeomData ParseHLine()const;
	GeomData ParseCircle()const;
	GeomData ParseCnum() const;
	

	float ExtractDecimal(char* buffer);
private:
	
	char* currentString;
	int cursor;
	int backbone;
	int endindex;
};