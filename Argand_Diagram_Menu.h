#pragma once
#include"Coniguration.h"

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

	void SetupDraw();

	char* GetStringFromEdit(children* Children);

	void SetupConverter(HWND Parent,std::vector<children>* children);

	void SwapConfigState(WPARAM wp, ConfigSetup* config);
	

	void Shag(HWND Parent, std::vector<children>* Children);
	
}


