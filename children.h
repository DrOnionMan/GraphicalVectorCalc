#pragma once
#include<windows.h>
#include"children.h"
#include"MacroUtils.h"
#include<vector>

enum childType {
	NONE = -1,
	STATIC = 1,
	EDIT,
	BUTTON,
	
};



struct children{

	HWND id;
	childType type;
	children* associate;
};


children* GetChild(children* (*fpOrder)(std::vector<children>* Children, childType Type), std::vector<children>* Children, childType Type);

std::vector<children> GetAllChildren(std::vector<children>* Children, childType Type);
int GetChildCount(std::vector<children>* Children, childType Type);

void Killchild(children* child);

children* first(std::vector<children>* Children, childType Type);
children* last(std::vector<children>* Children, childType Type);

void DeleteEdit(std::vector<children>* Children, int index);
