#include"children.h"
#include<string>


children* GetChild(std::vector<children>* Children, childType Type) {
	for (int i = 0; i < Children->size(); i++) {
		if (Children->at(i).type == Type) {
			return &Children->at(i);
		}
	}
}

std::vector<children>* GetAllChildren(std::vector<children>* Children, childType Type) {
	std::vector<children> school;
	for (int i = 0; i < Children->size(); i++) {
		if (Children->at(i).type == Type) {
			school.push_back(Children->at(i));
		}
	}

	return &school;
}


Friend CreateFriend(HWND id, childType type) {
	Friend f = { 0 };

	f.f_id = id;
	f.f_type = type;
	return f;
}