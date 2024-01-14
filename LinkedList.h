#pragma once
#include<stdio.h>
#include"GeometryData.h"
struct node {
	GeomData data;
	node* next;
};

void LogList(node* list);




void operator+=(node*& nodey, GeomData& data);

node* partition(node* first, node* last);
node* last_node(node* head);
void quick_sort(node* first, node* last);


//for costom sort other than overloaded < params
void quicksort_c(node* first, node* last, bool fn(node* right, node* left));
node* partition(node* first, node* last, bool fn(node* left, node* right));

void DeleteList(node* list);