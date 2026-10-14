#ifndef CIRCULAR_QUEUE_H
#define CIRCULAR_QUEUE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define CIRCULAR_QUEUE_CAPACITY 8

typedef struct {
    int taskId;
    char taskName[64];
    int assignedWorkerId;
} CircularTask;

typedef struct {
    CircularTask items[CIRCULAR_QUEUE_CAPACITY];
    int front;
    int rear;
    int count;
} CircularQueue;

void initCircularQueue(CircularQueue *cq);
bool isCircularQueueEmpty(CircularQueue *cq);
bool isCircularQueueFull(CircularQueue *cq);
bool enqueueCircularQueue(CircularQueue *cq, CircularTask task);
bool dequeueCircularQueue(CircularQueue *cq, CircularTask *outTask);
char* serializeCircularQueueJSON(CircularQueue *cq);

#endif // CIRCULAR_QUEUE_H
