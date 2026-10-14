#include "circular_queue.h"

void initCircularQueue(CircularQueue *cq) {
    if (cq) {
        cq->front = -1;
        cq->rear = -1;
        cq->count = 0;
    }
}

bool isCircularQueueEmpty(CircularQueue *cq) {
    return (cq == NULL || cq->count == 0);
}

bool isCircularQueueFull(CircularQueue *cq) {
    return (cq != NULL && cq->count >= CIRCULAR_QUEUE_CAPACITY);
}

bool enqueueCircularQueue(CircularQueue *cq, CircularTask task) {
    if (!cq || isCircularQueueFull(cq)) return false;
    if (cq->front == -1) cq->front = 0;
    cq->rear = (cq->rear + 1) % CIRCULAR_QUEUE_CAPACITY;
    cq->items[cq->rear] = task;
    cq->count++;
    return true;
}

bool dequeueCircularQueue(CircularQueue *cq, CircularTask *outTask) {
    if (!cq || isCircularQueueEmpty(cq)) return false;
    if (outTask) *outTask = cq->items[cq->front];
    cq->front = (cq->front + 1) % CIRCULAR_QUEUE_CAPACITY;
    cq->count--;
    if (cq->count == 0) {
        cq->front = -1;
        cq->rear = -1;
    }
    return true;
}

char* serializeCircularQueueJSON(CircularQueue *cq) {
    static char jsonBuf[2048];
    jsonBuf[0] = '\0';
    strcat(jsonBuf, "{\"capacity\":");
    char capStr[16];
    snprintf(capStr, sizeof(capStr), "%d,", CIRCULAR_QUEUE_CAPACITY);
    strcat(jsonBuf, capStr);
    strcat(jsonBuf, "\"count\":");
    snprintf(capStr, sizeof(capStr), "%d,", cq ? cq->count : 0);
    strcat(jsonBuf, capStr);
    strcat(jsonBuf, "\"front\":");
    snprintf(capStr, sizeof(capStr), "%d,", cq ? cq->front : -1);
    strcat(jsonBuf, capStr);
    strcat(jsonBuf, "\"rear\":");
    snprintf(capStr, sizeof(capStr), "%d,", cq ? cq->rear : -1);
    strcat(jsonBuf, capStr);
    strcat(jsonBuf, "\"items\":[");

    if (cq && cq->count > 0) {
        int idx = cq->front;
        for (int i = 0; i < cq->count; i++) {
            char itemBuf[256];
            snprintf(itemBuf, sizeof(itemBuf),
                     "{\"index\":%d,\"taskId\":%d,\"taskName\":\"%s\",\"workerId\":%d}",
                     idx, cq->items[idx].taskId, cq->items[idx].taskName, cq->items[idx].assignedWorkerId);
            strcat(jsonBuf, itemBuf);
            if (i < cq->count - 1) strcat(jsonBuf, ",");
            idx = (idx + 1) % CIRCULAR_QUEUE_CAPACITY;
        }
    }
    strcat(jsonBuf, "]}");
    return jsonBuf;
}
