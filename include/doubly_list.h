#ifndef DOUBLY_LIST_H
#define DOUBLY_LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct HistoryNode {
    int historyId;
    char timestamp[32];
    char operation[64];  // e.g. "SELECT_PRODUCT", "ADD_OPTION", "REMOVE_OPTION"
    int itemId;
    char itemName[100];
    double currentTotalPrice;
    struct HistoryNode *prev;
    struct HistoryNode *next;
} HistoryNode;

typedef struct {
    HistoryNode *head;
    HistoryNode *tail;
    HistoryNode *current;
    int count;
} DoublyHistoryList;

void initHistoryList(DoublyHistoryList *list);
void insertHistory(DoublyHistoryList *list, const char *operation, int itemId, const char *itemName, double totalPrice);
int deleteHistory(DoublyHistoryList *list, int historyId);
HistoryNode* previousState(DoublyHistoryList *list);
HistoryNode* nextState(DoublyHistoryList *list);
void displayHistory(DoublyHistoryList *list);
void clearHistory(DoublyHistoryList *list);
char* serializeHistoryJSON(DoublyHistoryList *list);

#endif // DOUBLY_LIST_H
