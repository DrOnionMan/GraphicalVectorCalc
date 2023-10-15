#include"Argand_Diagram_Menu.h"


Argand::Argand(HWND ParentWnd, ArgandConfig::UserInputType inputType) noexcept {
	m_State.InputType = inputType;
	m_State.ParentWindow = ParentWnd;

}

void Argand::SetUserChoice(int choice) {
	switch (choice) {
	case 0x4:
		m_State.UserChoice = ArgandConfig::ARGAND_CONVERTER;
		return;
	case 0x5:
		m_State.UserChoice = ArgandConfig::ARGAND_DRAWER;
		return;
	default:
		m_State.UserChoice = ArgandConfig::NULLCASE;
		MessageBox(NULL, "Error in SetUserChoice Function", "CodeErr", MB_ICONWARNING);
		break;
	}
}

void Argand::SetInputType(ArgandConfig::UserInputType type) {
	m_State.InputType = type;
}

void Argand::SetTitle(const std::string& Title, HWND hWnd) {
	SetWindowText(hWnd, Title.c_str());
}

Argand::~Argand() {

}