#ifndef ORDERED_LIST_H
#define ORDERED_LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct OrderedNode {
    int id;
    char name[100];
    double key; // e.g. price or score used for sorting
    struct OrderedNode *next;
} OrderedNode;

typedef struct {
    OrderedNode *head;
    int count;
} OrderedList;

void initOrderedList(OrderedList *list);
void insertSorted(OrderedList *list, int id, const char *name, double key);
OrderedNode* searchOrderedList(OrderedList *list, int id);
int deleteOrderedList(OrderedList *list, int id);
void freeOrderedList(OrderedList *list);
char* serializeOrderedListJSON(OrderedList *list);

#endif // ORDERED_LIST_H
