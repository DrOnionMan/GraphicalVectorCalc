#include"children.h"
#include<string>
#include"MacroUtils.h"

children* first(std::vector<children>* Children, childType Type) {
	for (int i = 0; i < Children->size(); i++) {
		if (Children->at(i).type == Type) {
			return &Children->at(i);
		}
	}
	return nullptr;
}

children* last(std::vector<children>* Children, childType Type) {
	for (int i = Children->size()-1; i >= 0 ; i--) {
		if (Children->at(i).type == Type) {
			return &Children->at(i);
		}
	}
	return nullptr;
}


children* GetChild(children* (*fpOrder)(std::vector<children>* Children, childType Type), std::vector<children>* Children, childType Type) {
	
	if (fpOrder) {
		return fpOrder(Children, Type);
	}

	return nullptr;
}

std::vector<children> GetAllChildren(std::vector<children>* Children, childType Type) {
	std::vector<children> school;
	for (int i = 0; i < Children->size(); i++) {
		if (Children->at(i).type == Type) {
			school.push_back(Children->at(i));
		}
	}

	return school;
}



int GetChildCount(std::vector<children>* Children, childType Type) {
	int count = 0;
	for (int i = 0; i < Children->size(); i++) {
		if (Children->at(i).type == Type) {
			count++;
		}
	}

	return count;
}

void DeleteEdit(std::vector<children>* Children, int index) {
	children child = Children->at(index);
	DestroyWindow(child.associate->id);
	DestroyWindow(child.id);
	Children->erase(std::next(Children->begin(), index));
	if (child.associate != nullptr) {
		delete child.associate;
	}
}


void Killchild(children* child) {
	DestroyWindow(child->id);
	if (child->associate) {
		DestroyWindow(child->associate->id);
	}
}