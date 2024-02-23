#include"Parser.h"
#include"MacroUtils.h"
#include"StringUtils.h"
#include"GeometryData.h"
#include<sstream>




void Parser::setParserText(char* text, unsigned int flag) {
	if (!text) {
		MB("NULLPTR ERROR IN PARSER");
		return;
	}
	if (!currentString) {
		currentString = text;
		if(!(flag & NOSAN))SanitiseInput();
		endindex = strlen(currentString);
	}
	else {
		free(currentString);
		currentString = text;
		if (!(flag & NOSAN))SanitiseInput();
		endindex = strlen(currentString);
	}
	if (flag == PROC) {
		ProcString();
	}
	
}

Parser::Parser() {

	currentString = nullptr;

	endindex = MAX_LEN;
	backbone = 0;
	cursor = 0;

}

Parser::~Parser() {
	free(currentString);
}

complex Parser::ParsetoComplex() {
	

	std::pair<int, int> x = FindToken("*E^");
	

	if (x.first < 0) {
		std::ostringstream oss;
		oss << "Token unfound\nExpected token -> " << "*e^";
		std::string s = oss.str();

		ERR("Parse Error", s.c_str());
		return { NULL };
	}

	char* mag = GetToken(0, x.first);
	char* exp = GetToken(x.second , endindex);


	if (!mag || !exp) {
		free(mag);
		free(exp);
		return { NULL };
	}

	if (!StringIsValid(mag, false)) {
		char* str = FindUnexpectedToken(mag, false);
		ERR("Unexpected Tokens Found in magnitude value", str);
		free(mag);
		free(exp);
		free(str);
		return { NULL };
	}

	if (!StringIsValid(exp, true)) {
		char* str = FindUnexpectedToken(exp, true);
		ERR("Unexpected Tokens Found in exponent value", str);
		free(str);
		free(mag);
		free(exp);
		return {NULL};
	}
	
	
	
	//Remove
	
	complex c = { 0 };

	
	c.magnitude = ExtractDecimal(mag);
	c.argument = ExtractDecimal(exp);
		
	

	return c;
}

void Parser::SanitiseInput() {
	for (char* p = currentString; *p != '\0'; p++) {
		if (INBOUND_INC('a', 'z', *p)) {
			TOUPPER(*p);
		}
	}
}

std::pair<int, int> Parser::FindToken(const char* token) {
	if (!token) {
		ERR("NULLPTR error", "allocation may have failed");
		return { -1, -1 };
	}

	int top = 0, bottom = 0;
	
	std::string s = token;
	int len = s.length() - 1;
	std::string cs = currentString;
	size_t found = cs.find(s);
	if (found != std::string::npos) {
		int i = 0;
		for (char& c : cs) {
			if (c == s[0]) {
				bottom = i;
			}
			if (c == s[len]) {
				top = i;
			}
			i++;
		}
		if (bottom > top) {
			return { -1, -1 };
		}
		else {
			return { bottom , top + 1 };
		}
	}
	else {
		return { -1, -1 };
	}
	
	

	
}

bool Parser::StringIsValid(char* text, bool isComplex) {
	std::string str = text;
	int len = str.length();
	if (isComplex) {
		int Uniquecount = 0;

		for (int i = 0; i < std::size(str); i++) {
			if (Uniquecount > 1) {
				return false;
			}
			switch (str[i]) {
			case '.':
				Uniquecount++;
				break;
			case '0':break; case '1':break; case '2':break; case '3':break; case '4':break; case '5':break; case '6':break; case '7':break; case '8':break; case '9':break;
			case 'I':
			{
				if (i == std::size(str) - 1) {
					str[i] = '0';
					continue;
				}
				else {
					return false;
				}
			}
			case '-':
				if (i == 0) {
					continue;
				}
				else {
					return false;
				}
				break;
			case '+':
				if (i == 0) {
					continue;
				}
				else {
					return false;
				}
				break;
			default:
				return false;
			}
		}
		return true;
	}
	else {
		//this is a count for the characters . & / there should not be more than 1 per string
		int Uniquecount = 0;

		for (int i = 0; i < std::size(str); i++) {
			if (Uniquecount > 1) {
				return false;
			}
			switch (str[i]) {
			case '.':
				Uniquecount++;
				break;
			case '0':
				break;
			case '1':
				break;
			case '2':
				break;
			case '3':
				break;
			case '4':
				break;
			case '5':
				break;
			case '6':
				break;
			case '7':
				break;
			case '8':
				break;
			case '9':
				break;
			case '+':
				if (i == 0) {
					continue;
				}
				else {
					return false;
				}
				break;

			default:
				return false;
			}
		}
	}
}

char* Parser::GetToken(int start, int end) {
	std::string s = currentString;
	std::string sub = s.substr(start, end);

	int tokenlen = sub.length();
	char* token = allocstr(tokenlen);
	
	strcpy(sub, tokenlen, token);
	
	return token;
}

float Parser::ExtractDecimal(char* buffer) {
	return atof(buffer);
}


GeomData Parser::ParseCircle() const {
	GeomData g = { 0 };
	
	if (4 != sscanf_s(currentString, "|%c-(%f%fi)|=%f", &g.name, 1,  &g.data.circle.center_x, &g.data.circle.center_y, &g.data.circle.radius)) {
		ERR("Syntax Error", "Invalid format for circle geometry");
		g.gType = NULLTYPE;
		return g;
	}
	g.gType = CIRCLE;

	return g;
}


GeomData Parser::ParseHLine() const {
	GeomData g = { 0 };

	if (4 != sscanf_s(currentString, "Arg(%c-(%f%fi))=%f", &g.name, 1,&g.data.hline.xspos, &g.data.hline.yspos, &g.data.hline.arg)) {
		
		ERR("Syntax Error", "Invalid format for half-line geometry");
		g.gType = NULLTYPE;
		return g;
	}

	g.gType = HLINE;

	return g;
}


GeomData Parser::ParseCnum() const {
	GeomData g = { 0 };
	
	if (3 != sscanf_s(currentString, "%c=%f*e^%fi", &g.name, 1, &g.data.comp_num.mag, &g.data.comp_num.arg)) {
		
		ERR("Syntax Error", "Invalid format for complex number geometry");
		g.gType = NULLTYPE;
		return g;
	}

	g.gType = CNUM;

	return g;
}


GeomData Parser::ProcExprStrToGeom(char* txt) {
	GeomData g = { NULL };
	
	setParserText(txt, NOPROC | NOSAN);
	std::string expression = txt;
	if (expression.find("Arg") != std::string::npos) {
		g = ParseHLine();
		return g;
	}
	if (expression.find("|") != std::string::npos) {
		g = ParseCircle();
		return g;
	}
	if (expression.find("*e^") != std::string::npos) {
		g = ParseCnum();
		return g;
	}

	float a = 0.0f, b = 0.0f, c = 0.0f, d = 0.0f;

	if (
		4 == sscanf_s(txt,"%fx%fy%fz=%fd", &a, &b, &c, &d) 
		|| 
		3 == sscanf_s(txt, "0x%fy%fz=%fd", &b, &c, &d)
		) {
		
		

		g.data.mat.a = a;
		g.data.mat.b = b;
		g.data.mat.c = c;
		g.data.mat.d = d;


		
		g.gType = MAT;
		
		return g;
	}
	
	g.gType = NULLTYPE;
	ERR("Syntax Error", "Invalid expression");
	return g;
}

void Parser::ProcString() {
	if (currentString) {
		
	}
	else {
		MB("NULL String");
	}
	return;
}