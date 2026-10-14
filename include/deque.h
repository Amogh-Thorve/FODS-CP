#ifndef DEQUE_H
#define DEQUE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define DEQUE_CAPACITY 20

typedef struct {
    int priority; // 1 = High, 2 = Medium, 3 = Low
    char description[64];
} DequeItem;

typedef struct {
    DequeItem items[DEQUE_CAPACITY];
    int front;
    int rear;
    int count;
} Deque;

void initDeque(Deque *dq);
bool isDequeEmpty(Deque *dq);
bool isDequeFull(Deque *dq);
bool insertFrontDeque(Deque *dq, DequeItem item);
bool insertRearDeque(Deque *dq, DequeItem item);
bool deleteFrontDeque(Deque *dq, DequeItem *outItem);
bool deleteRearDeque(Deque *dq, DequeItem *outItem);
char* serializeDequeJSON(Deque *dq);

#endif // DEQUE_H
