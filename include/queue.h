#ifndef QUEUE_H
#define QUEUE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_QUEUE_SIZE 100

typedef struct {
    int jobId;
    char jobType[64];  // "VALIDATION", "PRICING", "QUOTE_GEN"
    char details[128];
    int configId;
    char status[32];   // "PENDING", "PROCESSING", "COMPLETED"
} QuoteJob;

// Array-based Queue
typedef struct {
    QuoteJob jobs[MAX_QUEUE_SIZE];
    int front;
    int rear;
    int count;
} ArrayQueue;

void initArrayQueue(ArrayQueue *q);
bool isArrayQueueEmpty(ArrayQueue *q);
bool isArrayQueueFull(ArrayQueue *q);
bool enqueueArrayQueue(ArrayQueue *q, QuoteJob job);
bool dequeueArrayQueue(ArrayQueue *q, QuoteJob *outJob);
QuoteJob* peekArrayQueue(ArrayQueue *q);

// Linked List Queue
typedef struct QueueNode {
    QuoteJob job;
    struct QueueNode *next;
} QueueNode;

typedef struct {
    QueueNode *front;
    QueueNode *rear;
    int count;
} ListQueue;

void initListQueue(ListQueue *q);
bool isListQueueEmpty(ListQueue *q);
void enqueueListQueue(ListQueue *q, QuoteJob job);
bool dequeueListQueue(ListQueue *q, QuoteJob *outJob);
QuoteJob* peekListQueue(ListQueue *q);
void clearListQueue(ListQueue *q);

#endif // QUEUE_H
