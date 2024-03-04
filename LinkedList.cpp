#include"LinkedList.h"
#include<stdlib.h>
#include"MacroUtils.h"
#include<assert.h>
#include<fstream>

void LogList(node* list) {
	for (node* l = list; l != NULL; l = l->next) {
		char c[2] = { l->data.name, '\0'};
		MB(c);
	}
}


//node* taken by reference because otherwise it doesnt work
//Take geomData by ref also, reduces copying
void operator+=(node*& nodey, GeomData& data) {
	//stack allocate new node
	node* new_node;
	//assign it to a heap allocation
	new_node = (node*)malloc(sizeof(node));
	//simple assert
	assert(new_node != NULL);
	//Copy data into the node
	new_node->data = data;
	//this node becomes head of list, old head pushed back a spot
	new_node->next = nodey;
	//pointer to head of list becomes pointer to new head.
	//if the node* is not takem by reference then this is where weird stuff happens
	//i think the list is assigned to a local pointer when scope is out this is then cleaned up
	//meaning weve created a dangling pointer to a linked list
	//could additionally be taken by node** however this would require syntax like
	//&list += gdata which io dont like so its taken by node*& instead
	nodey = new_node;
}




//just gets the last node :))
node* last_node(node* head){
	node* temp = head;
	while (temp != NULL && temp->next != NULL) {
		temp = temp->next;
	}
	return temp;
}


node* partition(node* first, node* last, bool fn(node* left, node* right)){
	// get first node
	node* pivot = first;
	node* front = first;
	GeomData temp = {};
	while (front != NULL && front != last) {
		//comparison relies on function pointer / callback function
		if (fn(front, last)) {
			pivot = first;
			// Swapping  node values 
			temp = first->data;
			first->data = front->data;
			front->data = temp;
			// goto next node
			first = first->next;
		}
		// goto next node
		front = front->next;
	}
	//not null check
	assert(first != NULL);
	// Change last node value to current node 
	temp = first->data;
	first->data = last->data;
	last->data = temp;
	return pivot;
}


void quicksort_c(node* first, node* last, bool fn(node* right, node* left)) {
	//dont do if its 1 node long ........
	if (first == last) {
		return;
	}
	//split the list
	node* pivot = partition(first, last, fn);
	if (pivot != NULL && pivot->next != NULL) {
		//recursive call
		quicksort_c(pivot->next, last, fn);
	}
	if (pivot != NULL && first != pivot) {
		//same here
		quicksort_c(first, pivot, fn);
	}
}
#include"StringUtils.h"

void DeleteList(node* list) {
	//exit condition
	if (list == NULL) {
		return;
	}
	//recursive call
	DeleteList(list->next);
	//delete the list obvs
	free(list);
}