#include"Parser.h"
#include"MacroUtils.h"
#include"StringUtils.h"
#include"GeometryData.h"
#include<sstream>




void Parser::setParserText(char* text, unsigned int flag) {
	if (!text) {
		ERR("UNDEFINED ERROR", "NULLPTR ERROR IN PARSER");
		return;
	}
	if (!currentString) {
		currentString = text;
		if(!(flag & NOSAN)) SanitiseInput();
		endindex = strlen(currentString);
	}
	else {
		free(currentString);
		currentString = text;
		if (!(flag & NOSAN)) SanitiseInput();
		endindex = strlen(currentString);
	}
	if (flag == PROC) {
		ProcString();
	}
	
}

Parser::Parser() {

	currentString = nullptr;

	endindex = MAX_LEN;

}

Parser::~Parser() {
	if(currentString)
	free(currentString);
}

complex Parser::ParsetoComplex() {
	//Finds the index of the token * and token ^ in the string
	//makes sure the e is there aswell however, splits the string into 2 float values
	//If the token isnt found retunrd pair is {-1, -1}
	std::pair<int, int> x = FindToken("*E^");
	
	//Check to see if the above func failed
	if (x.first < 0) {
		//throw error message in a text box
		std::ostringstream oss;
		oss << "Token unfound\nExpected token -> " << "*e^";
		std::string s = oss.str();
		ERR("Parse Error", s.c_str());
		//return NULL struct
		return { NULL };
	}
	//else we get the numbers
	//as in the form r*e^argi
	//mag = r hence the string is index 0 to where the * token is
	//same for the arg value only start index is the ^
	char* mag = GetToken(0, x.first);
	char* exp = GetToken(x.second , endindex);
	//if either is a nullpointer free both and return null
	if (!mag || !exp) {
		free(mag);
		free(exp);
		return { NULL };
	}
	//Checks to see if the string is valid checks for unexpected tokens
	//eg as it is a number no characters (a, b, c, d..) should be present
	//only 1 . etc
	if (!StringIsValid(mag, false)) {
		//Gets all the unexpected tokens and puts them in the buffer str
		char* str = FindUnexpectedToken(mag, false);
		//Displays all the unexpected values
		ERR("Unexpected Tokens Found in magnitude value", str);
		//ofc free all dynamically allocated buffers && return NULL
		free(mag);
		free(exp);
		free(str);
		return { NULL };
	}
	//see aboove
	if (!StringIsValid(exp, true)) {
		char* str = FindUnexpectedToken(exp, true);
		ERR("Unexpected Tokens Found in exponent value", str);
		free(str);
		free(mag);
		free(exp);
		return {NULL};
	}
	//create empty struct
	complex c = { 0 };
	//Extract float values from the strings, just calls atoll function
	c.magnitude = ExtractDecimal(mag);
	c.argument = ExtractDecimal(exp);
	//one last logic check
	if (c.magnitude < 0.0f) return {NULL};
	//return found Complex number
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
	//for details see ParseCnum
	if (4 != sscanf_s(currentString, "|%c-(%f%fi)|=%f", &g.name, 1,  &g.data.circle.center_x, &g.data.circle.center_y, &g.data.circle.radius)) {
		ERR("Syntax Error", "Invalid format for circle geometry");
		g.gType = NULLTYPE;
		return g;
	}
	//Radius cannot be negative, covers edge case.
	if (g.data.circle.radius < 0.0f) {
		ERR("Syntax Error", "Invalid format for circle geometry");
		g.gType = NULLTYPE;
		return g;
	}
	g.gType = CIRCLE;

	return g;
}


GeomData Parser::ParseHLine() const {
	GeomData g = { 0 };
	//for details see ParseCnum
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
	//if the things arnt scanned in / if there is an error
	if (3 != sscanf_s(currentString, "%c=%f*e^%fi", &g.name, 1, &g.data.comp_num.mag, &g.data.comp_num.arg)) {
		ERR("Syntax Error", "Invalid format for complex number geometry");
		g.gType = NULLTYPE;
		return g;
	}
	//obviously the magnitude cant be less than 0 makes no sense
	//cover this edge case.
	if (g.data.comp_num.mag < 0.0f) {
		//More detailed error notes
		ERR("Syntax Error", "Invalid format for complex number geometry");
		g.gType = NULLTYPE;
		return g;
	}
	g.gType = CNUM;
	//else we return g.
	return g;
}


GeomData Parser::ProcExprStrToGeom(char* txt) {
	GeomData g = { NULL };
	//set my text to the buffer passed in
	//dont capitalise it
	setParserText(txt, NOPROC | NOSAN);
	//put the text buffer into a std::string
	std::string expression = txt;
	//if statements, if the substring is in the main string
	//npos wont be returned and we can process them as we like.

	//Formats for entry are
	/*
		<character>=<float>*e^<float>i
		Arg(<character>-(<float><operator(+|-)><float>i))=<float>
		|<character>-(<float><operator(+|-)><float>i)|=<float>
	*/
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
	//Matrix implementation. Just as bove
	if (
		4 == sscanf_s(txt,"%fx%fy%fz=%fd", &a, &b, &c, &d) 
		|| 
		3 == sscanf_s(txt, "0x%fy%fz=%fd", &b, &c, &d)
		) {
		
		if (b == 0.0f && c == 0.0f && a == 0.0f) {
			goto Error;
		}

		g.data.mat.a = a;
		g.data.mat.b = b;
		g.data.mat.c = c;
		g.data.mat.d = d;


		
		g.gType = MAT;
		
		return g;
	}

Error:
	g.gType = NULLTYPE;
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