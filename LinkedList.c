#include <stdio.h>
#include <stdlib.h>

#ifndef _LINKED_LIST_REALIZATION_
#define _LINKED_LIST_REALIZATION_

typedef struct Node{
	struct Node* next;
	int value;
} Node;

Node* createNewNode(int index){
	Node* newNode = malloc(sizeof(Node));
	if (newNode == NULL){
		printf("Memory allocation error! (In LinkedList modeule)\n");
		exit(1);
	}
	newNode->value = index;
	newNode->next = NULL;
	return newNode;
}

Node* destroyNode(Node* anyNode){
	Node* nextNode = anyNode->next;
	free(anyNode);
	return nextNode;
}

typedef struct{
	Node* head;
} LinkedIntList;

LinkedIntList* createNewLinkedIntList(){
	LinkedIntList* list = malloc(sizeof(LinkedIntList));
	if (list == NULL){exit(1);}
	list->head = NULL;
	return list;
}

int destroyLinkedIntList(LinkedIntList* list){
	Node* currentNode = list->head;
	while (currentNode != NULL){
		currentNode = destroyNode(currentNode);
	}
	free(list);
	return 0;
}

void printLinkedIntList(LinkedIntList* list){
	Node* currentNode = list->head;
	while (currentNode != NULL){
		if (currentNode->next == NULL){
			printf("%d\n", currentNode->value);
			break;
		}
		printf("%d, ", currentNode->value);
		currentNode = currentNode->next;
	}	
}

int appendValue(LinkedIntList* list, int value){
	Node* headPointer = list->head;
	Node* newHead = createNewNode(value);
	list->head = newHead;
	list->head->next = headPointer;
	return 0;
}

int removeValue(LinkedIntList* list, int value){
	Node* currentNode = list->head;
	Node* preNode;
	if (currentNode == NULL){return -1;}
	if (currentNode->value == value){
		list->head = currentNode->next;
		destroyNode(currentNode);
		return 0;
	}
	if (currentNode->next != NULL && currentNode->next->value == value){
		preNode = currentNode;
		currentNode = currentNode->next;
		preNode->next = currentNode->next;
		destroyNode(currentNode);
		return 0;
	}
	while (currentNode != NULL){
		if (currentNode->value == value){
			preNode->next = currentNode->next;
			destroyNode(currentNode);
			return 0;
		}
		preNode = currentNode;
		currentNode = currentNode->next;
	}
	return -1;
}

#endif