#include"Argand_Diagram_Menu.h"
#include"MacroUtils.h"
#include"StringUtils.h"
#include<assert.h>
#include <algorithm>
#include <array>
#include <functional>
#include <string_view>




void Argand::SetupConverter(HWND Parent, std::vector<children>*__restrict Children) {
	children temp = { 0 };

	temp.associate = nullptr;

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
	if (Children == nullptr || Children->type != EDIT) {
		//if the window is nullptr break ofc!!!
		//same for if its not an edit
		return nullptr;
	}
	//allocate the buffer
	char* text = allocstr(MAX_LEN);
	if (text) {
		//fill the buffer
		//annoying that it tells you the size of the buffer after youve allocated the string!!
		//would be nice to know that before
		GetWindowText(Children->id, text, MAX_LEN);
		//return the contents of the edit
		return text;
	}
	//if the buffer is a nullptr return nullptr OFC!
	return nullptr;
}

void Argand::SetupDraw(HWND Parent, std::vector<children>* __restrict Children) {
	//create child windows.
	children draw_btn = { NULL };
	draw_btn.type = BUTTON;
	//create button.
	draw_btn.id = CreateWindow("Button",
		"Draw",
		WS_VISIBLE | WS_CHILD | WS_BORDER,
		650, 150, 75, 30,
		//odd cast to HMENU Needed but i not sure why
		//What message is sent when button is pressed
		//      *                *        
		Parent, (HMENU)DRAW_ARGAND, NULL, NULL
	);
	//no associate window so set to nullptr
	draw_btn.associate = nullptr;
	//add to window buffer
	Children->push_back(draw_btn);


	//asme as above
	children AddGeom_btn = { NULL };
	AddGeom_btn.type = BUTTON;
	AddGeom_btn.id = CreateWindow("Button",
		"Add",
		WS_VISIBLE | WS_CHILD | WS_BORDER,
		650, 50, 75, 30,
		Parent, (HMENU)ADD_COMPLEX_GEOMETRY, NULL, NULL
	);
	AddGeom_btn.associate = nullptr;
	Children->push_back(AddGeom_btn);

}

typedef uint64_t ui64;
void Argand::DisplayConverterResult(complex& complex, std::vector<children>*__restrict Children, HWND Parent) {
	children MOD = { NULL };
	MOD.associate = nullptr;

	char MODBuffer[MAX_LEN];


	sprintf_s(MODBuffer, "Modulus Argument Form:\n%.3f*(Cos(%.3f) + iSin(%.3f))", complex.magnitude, complex.argument, complex.argument);

	MOD.id = CreateWindow("Static",
		MODBuffer,
		WS_VISIBLE | WS_CHILD | WS_BORDER,
		400, 50, 300, 60,
		Parent, NULL, NULL, NULL
	);
	MOD.type = STATIC;
	Children->push_back(MOD);

	children ABI = { NULL };
	ABI.associate = nullptr;
	char ABIBuffer[MAX_LEN];
	sprintf_s(ABIBuffer, "A+Bi Form:\n(%.2f) + (%.2f)i", cos(complex.argument) *complex.magnitude, sin(complex.argument) * complex.magnitude);
	
	ABI.id = CreateWindow("Static",
		ABIBuffer,
		WS_VISIBLE | WS_CHILD | WS_BORDER,
		400, 150, 300, 60,
		Parent, NULL, NULL, NULL
	);
	ABI.type = STATIC;
	Children->push_back(ABI);
}

children* CreateDeleteButton(HWND Parent,HWND associate, std::vector<children>*__restrict Children, int index, RECT* rect) {
	/*TODO FIGURE OUT HOW TO DO THIS SHIT */
	children* DeleteButton = new children;
	if (rect == nullptr) {
		
		DeleteButton->type = BUTTON;
		DeleteButton->id = CreateWindow("Button",
			"Delete",
			WS_VISIBLE | WS_CHILD | WS_BORDER | BS_DEFPUSHBUTTON,
			475, 50, 75 , 30,
			Parent, NULL, NULL, NULL
		);
		
		DeleteButton->associate = nullptr;
		assert(DeleteButton != nullptr);
		return DeleteButton;
	}
	else {
		
		DeleteButton->type = BUTTON;
		DeleteButton->id = CreateWindow("Button",
			"Delete",
			WS_VISIBLE | WS_CHILD | WS_BORDER | BS_DEFPUSHBUTTON,
			475, rect->bottom + 20, 75, 30,
			Parent, NULL, NULL, NULL
		);

		DeleteButton->associate = nullptr;
		assert(DeleteButton != nullptr);
		return DeleteButton;

	}
}



static void GetChildRect(RECT* rect, const HWND& hWnd) {
	GetWindowRect(hWnd, rect);
	MapWindowPoints(HWND_DESKTOP, GetParent(hWnd), (LPPOINT)rect, 2);
}


RECT Argand::EmplaceEdit(std::vector<children> * __restrict Children) {
	//Pretty self explanitory here gets all the edits from my buffer
	std::vector<children> Edits = GetAllChildren(Children, EDIT);
	//if the buffsize is 0 (no edits on window return the top most rect)
	if (Edits.size() == 0) {
		RECT rect = { NULL };
		rect.left = 50;
		rect.top = 50;
		return { rect };
	}

	//Get rects of all edits
	std::vector<RECT> rects;
	//simple foreach
	for (const auto& edit : Edits) {
		RECT rect = { NULL };
		//use of getchildrect
		GetChildRect(&rect, edit.id);
		rects.push_back(rect);
	}
	
	//lambda expression no capture group defined
	const auto LT = [](const RECT& a, const RECT& b) {
		//return true if a is lower than b
		return a.bottom < b.bottom ? true : false;
	};
	//use of std::sort
	std::sort(rects.begin(), rects.end(), LT);
	//if the top most found rect is not equal to the top most possible rect then return 
	//coords of top most possible rect
	if ((*rects.begin()).bottom != 50) {
		RECT rect = { NULL };
		rect.left = 50;
		rect.top = 50;
		return { rect };
	}
	//If buff is greater than 2
	if (rects.size() >= 2) {
		for (int i = 0; i < rects.size() - 1; i++) {
			if (rects.at(i).bottom == 0) {
				continue;
			}
			//return rect of first gap found in rect list
			if (rects.at(i).bottom + 50 != rects.at(i + 1).bottom) {
				return rects.at(i);
			}

		}
	}//Botttom 2 cover edge cases
	if (rects.size() == 1) {
		return rects.at(0);
	}
	else {
		return rects.at(rects.size() - 1);
	}
	
}

void Argand::AddGeometryBox(HWND Parent, std::vector<children>* __restrict Children) {
	int i = Children->size();
	

	//emplace edit function
	RECT rect = EmplaceEdit(Children);
	
	children EditBox = { NULL };
	EditBox.type = EDIT;
	EditBox.id = CreateWindow("Edit",
		"",
		//ES_AUTOSCROLL so that it will scroll horezonatlly
		//for large input strings
		WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL,
		//add 20 pixel padding between the edits
		rect.left, rect.bottom + 20, 350, 30,
		Parent, (HMENU)ADD_COMPLEX_GEOMETRY, NULL, NULL
	);
	//Adds a delete button
	EditBox.associate = CreateDeleteButton(Parent, EditBox.id, Children, 0, &rect);
	Children->push_back(EditBox);
	return;
}



void Mats::SetupMats(HWND Parent, std::vector<children>* __restrict Children) {
	children temp = { 0 };

	temp.associate = nullptr;

	temp.id = CreateWindow("Static",
		"Enter Matricies in the form ax+by+cz=d",
		WS_VISIBLE | WS_CHILD | WS_BORDER,
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

	temp.id = CreateWindow("Edit",
		"",
		WS_VISIBLE | WS_CHILD | WS_BORDER,
		10, 70, 300, 25,
		Parent, NULL, NULL, NULL
	);
	temp.type = EDIT;
	Children->push_back(temp);

	temp.id = CreateWindow("Edit",
		"",
		WS_VISIBLE | WS_CHILD | WS_BORDER,
		10, 100, 300, 25,
		Parent, NULL, NULL, NULL
	);
	temp.type = EDIT;
	Children->push_back(temp);

	temp.id = CreateWindow("Button",
		"Draw",
		WS_VISIBLE | WS_CHILD | WS_BORDER,
		400, 100, 75, 30,
		Parent, (HMENU)M_TRANSMIT, NULL, NULL
	);
	temp.type = BUTTON;

	Children->push_back(temp);
}
