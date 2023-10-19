#pragma once
#include<windows.h>
#include"children.h"
#include<vector>

enum childType {
	STATIC,
	EDIT,
	BUTTON,
	NONE
};

typedef struct {
	HWND f_id;
	childType f_type;
} Friend;

typedef struct {
	HWND id;
	childType type;
	Friend associate;
} children;


children* GetChild(std::vector<children>* Children, childType Type);

std::vector<children>* GetAllChildren(std::vector<children>* Children, childType Type);

Friend CreateFriend(HWND id, childType type);