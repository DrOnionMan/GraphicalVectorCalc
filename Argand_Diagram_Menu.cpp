#include"Argand_Diagram_Menu.h"




void Argand::SwapConfigState(WPARAM wp, ConfigSetup* config) {
	switch (wp) {
	case A_BI:
		config->ArgandInputType = ArgandConfig::ABI;
		break;
	case MODARG:
		config->ArgandInputType = ArgandConfig::MOD_ARG;
		break;
	case EI:
		config->ArgandInputType = ArgandConfig::REI;
		break;
	}
}




void Argand::SetupConverter(HWND Parent, std::vector<children>* Children) {
	children temp = { 0 };

	temp.associate = CreateFriend(NULL, NONE);

	temp.id = CreateWindow("Static",
		"Enter The Complex Number To Be Converted: ",
		WS_VISIBLE | WS_CHILD | WS_BORDER ,
		10, 10, 300, 25,
		Parent, NULL, NULL, NULL
		);
	temp.type = STATIC;
	Children->push_back(temp);

	temp.id = CreateWindow("Edit",
		"",
		WS_VISIBLE | WS_CHILD | WS_BORDER,
		10, 40, 300, 25,
		Parent, NULL, NULL, NULL
	);
	temp.type = EDIT;
	Children->push_back(temp);

	temp.id = CreateWindow("Button",
		"Convert",
		WS_VISIBLE | WS_CHILD | WS_BORDER,
		100, 100, 75, 30,
		Parent, (HMENU) CONVERT , NULL, NULL
	);
	temp.type = BUTTON;

	Children->push_back(temp);
}

char* Argand::GetStringFromEdit(children* Children) {
	if (Children == nullptr) {
		return nullptr;
	}
	if (Children->type != EDIT) {
		return nullptr;
	}

	char* text = (char*)malloc(sizeof(char)*MAX_LEN);
	if (text) {
		memset(text, '\0', sizeof(char) * MAX_LEN);
		GetWindowText(Children->id, text, MAX_LEN);

		return text;
	}


	return nullptr;
}

void Argand::Shag(HWND Parent, std::vector<children>* Children) {
	children temp = { 0 };

	temp.id = CreateWindow("Static",
		"Enter The Complex Number To Be Converted: ",
		WS_VISIBLE | WS_CHILD | WS_BORDER,
		10, 10, 300, 25,
		Parent, NULL, NULL, NULL
	);
	temp.type = STATIC;
	Children->push_back(temp);
}