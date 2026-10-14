#include "queue.h"

// Array Queue Implementation
void initArrayQueue(ArrayQueue *q) {
    if (q) {
        q->front = 0;
        q->rear = -1;
        q->count = 0;
    }
}

bool isArrayQueueEmpty(ArrayQueue *q) {
    return (q == NULL || q->count == 0);
}

bool isArrayQueueFull(ArrayQueue *q) {
    return (q != NULL && q->count >= MAX_QUEUE_SIZE);
}

bool enqueueArrayQueue(ArrayQueue *q, QuoteJob job) {
    if (!q || isArrayQueueFull(q)) return false;
    q->rear = (q->rear + 1) % MAX_QUEUE_SIZE;
    q->jobs[q->rear] = job;
    q->count++;
    return true;
}

bool dequeueArrayQueue(ArrayQueue *q, QuoteJob *outJob) {
    if (!q || isArrayQueueEmpty(q)) return false;
    if (outJob) *outJob = q->jobs[q->front];
    q->front = (q->front + 1) % MAX_QUEUE_SIZE;
    q->count--;
    return true;
}

QuoteJob* peekArrayQueue(ArrayQueue *q) {
    if (!q || isArrayQueueEmpty(q)) return NULL;
    return &q->jobs[q->front];
}

// List Queue Implementation
void initListQueue(ListQueue *q) {
    if (q) {
        q->front = NULL;
        q->rear = NULL;
        q->count = 0;
    }
}

bool isListQueueEmpty(ListQueue *q) {
    return (q == NULL || q->front == NULL);
}

void enqueueListQueue(ListQueue *q, QuoteJob job) {
    if (!q) return;
    QueueNode *node = (QueueNode*)malloc(sizeof(QueueNode));
    if (!node) return;
    node->job = job;
    node->next = NULL;
    if (q->rear) {
        q->rear->next = node;
    } else {
        q->front = node;
    }
    q->rear = node;
    q->count++;
}

bool dequeueListQueue(ListQueue *q, QuoteJob *outJob) {
    if (!q || isListQueueEmpty(q)) return false;
    QueueNode *temp = q->front;
    if (outJob) *outJob = temp->job;
    q->front = q->front->next;
    if (!q->front) q->rear = NULL;
    free(temp);
    q->count--;
    return true;
}

QuoteJob* peekListQueue(ListQueue *q) {
    if (!q || isListQueueEmpty(q)) return NULL;
    return &q->front->job;
}

void clearListQueue(ListQueue *q) {
    if (!q) return;
    QuoteJob dummy;
    while (!isListQueueEmpty(q)) {
        dequeueListQueue(q, &dummy);
    }
}
