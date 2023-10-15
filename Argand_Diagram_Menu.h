#pragma once
#include"Coniguration.h"
#include<string>



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
	void SetupConverter();
	void SetTitle(const std::string& name, HWND hWnd);

	State m_State;
};


