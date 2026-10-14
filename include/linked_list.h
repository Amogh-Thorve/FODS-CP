#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct ConfigNode {
    int itemId;         // productId or optionId
    char name[100];
    char category[50];
    double price;
    int isOption;       // 0 for base product, 1 for option
    struct ConfigNode *next;
} ConfigNode;

typedef struct {
    ConfigNode *head;
    int count;
} ConfigLinkedList;

void initLinkedList(ConfigLinkedList *list);
ConfigNode* createNode(int itemId, const char *name, const char *category, double price, int isOption);
void insertFront(ConfigLinkedList *list, int itemId, const char *name, const char *category, double price, int isOption);
void insertEnd(ConfigLinkedList *list, int itemId, const char *name, const char *category, double price, int isOption);
void insertAtPosition(ConfigLinkedList *list, int position, int itemId, const char *name, const char *category, double price, int isOption);
int deleteFront(ConfigLinkedList *list);
int deleteEnd(ConfigLinkedList *list);
int deleteByValue(ConfigLinkedList *list, int itemId);
ConfigNode* searchNode(ConfigLinkedList *list, int itemId);
int updateNode(ConfigLinkedList *list, int itemId, double newPrice);
void displayList(ConfigLinkedList *list);
int countNodes(ConfigLinkedList *list);
void clearList(ConfigLinkedList *list);
char* serializeLinkedListJSON(ConfigLinkedList *list);

#endif // LINKED_LIST_H
