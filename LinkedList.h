#pragma once
#include<stdio.h>
#include"GeometryData.h"
struct node {
	GeomData data;
	node* next;
};

void LogList(node* list);



void PrintList(node* list);

void operator+=(node*& nodey, GeomData& data);

node* partition(node* first, node* last);
node* last_node(node* head);
void quick_sort(node* first, node* last);

void DeleteList(node* list);