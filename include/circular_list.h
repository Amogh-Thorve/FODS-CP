#ifndef CIRCULAR_LIST_H
#define CIRCULAR_LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct CircularNode {
    int workerId;
    char workerName[50];
    char currentTask[100];
    struct CircularNode *next;
} CircularNode;

typedef struct {
    CircularNode *head;
    CircularNode *tail;
    CircularNode *currentWorker;
    int count;
} CircularList;

void initCircularList(CircularList *list);
void insertWorker(CircularList *list, int id, const char *name);
CircularNode* getNextWorker(CircularList *list);
void displayCircularList(CircularList *list);
void clearCircularList(CircularList *list);
char* serializeCircularListJSON(CircularList *list);

#endif // CIRCULAR_LIST_H
