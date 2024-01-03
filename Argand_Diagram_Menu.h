#pragma once
#include"Coniguration.h"
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

	void SetupDraw(HWND Parent, std::vector<children>* children);

	char* GetStringFromEdit(children* Children);

	void SetupConverter(HWND Parent,std::vector<children>* children);

	void SwapConfigState(WPARAM wp, ConfigSetup* config);
	
	void DisplayConverterResult(complex& complex, std::vector<children>* Children, HWND Parent);
	
	RECT EmplaceEdit(std::vector<children>* Children);


	
	void AddGeometryBox(HWND Parent, std::vector<children>* children);
	
}


