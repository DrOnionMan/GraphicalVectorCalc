#pragma once

#include"Complex.h"
#include"children.h"
#include"MenuMacroDef.h"
#include<string>
#include<vector>


/*
class Argand  {

	
private:
	struct State {
		ArgandConfig::UserInput UserChoice;
		HWND ParentWindow;
		ArgandConfig::UserInputType InputType;
	};
public:
	Argand(HWND ParentWnd, ArgandConfig::UserInputType inputType) noexcept;
	~Argand() noexcept;


	void SetInputType(ArgandConfig::UserInputType type);
	void SetUserChoice(int choice);
	void SetupDraw();
	void SetupConverter(std::vector<HWND> *children);
	void SetTitle(const std::string& name, HWND hWnd);

	State m_State;
};
*/

namespace Argand {

	void SetupDraw(HWND Parent, std::vector<children>*__restrict children);

	char* GetStringFromEdit(children* Children);

	void SetupConverter(HWND Parent,std::vector<children>* __restrict children);

	
	void DisplayConverterResult(complex& complex, std::vector<children>* __restrict Children, HWND Parent);
	
	RECT EmplaceEdit(std::vector<children>* __restrict Children);


	
	void AddGeometryBox(HWND Parent, std::vector<children>* __restrict children);
	
}

namespace Mats {
	void SetupMats(HWND Parent, std::vector<children>* __restrict children);
}


