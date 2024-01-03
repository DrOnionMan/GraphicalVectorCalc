#include"LinkedList.h"
#include<stdlib.h>
#include<assert.h>
#include<fstream>

void LogList(node* list) {
	std::ofstream myFile;
	
	myFile.open("C:/A level Computing/GraphicalVectorCalc/Log.txt");
	
	for (node* p = list; p != NULL; p = p->next) {
		myFile << p->data.name;
		if (p->next != NULL) {
			myFile << " , ";
		}
	}
	myFile << std::endl;
	myFile.close();
}

void operator+=(node*& nodey, GeomData& data) {
	node* new_node;
	new_node = (node*)malloc(sizeof(node));
	assert(new_node != NULL);
	new_node->data = data;
	new_node->next = nodey;


	nodey = new_node;
}

bool operator<(GeomData g1, GeomData g2) {
	return g1.name < g2.name ? true : false;
}


node* partition(node* first, node* last)
{
	// Get first node of given linked list 
	node* pivot = first;
	node* front = first;
	GeomData temp = {};
	while (front != NULL && front != last) {
		if (front->data < last->data) {
			pivot = first;

			// Swapping  node values 
			temp = first->data;
			first->data = front->data;
			front->data = temp;

			// Visiting the next node 
			first = first->next;
		}

		// Visiting the next node 
		front = front->next;
	}
	assert(first != NULL);
	// Change last node value to current node 
	temp = first->data;
	first->data = last->data;
	last->data = temp;
	return pivot;
}
node* last_node(node* head)
{
	node* temp = head;
	while (temp != NULL && temp->next != NULL) {
		temp = temp->next;
	}
	return temp;
}
void quick_sort(node* first, node* last)
{
	if (first == last) {
		return;
	}
	node* pivot = partition(first, last);

	if (pivot != NULL && pivot->next != NULL) {
		quick_sort(pivot->next, last);
	}

	if (pivot != NULL && first != pivot) {
		quick_sort(first, pivot);
	}
}

void DeleteList(node* list) {
	if (list == NULL) {
		return;
	}
	DeleteList(list->next);
	delete(list);
}