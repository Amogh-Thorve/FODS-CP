#include "deque.h"

void initDeque(Deque *dq) {
    if (dq) {
        dq->front = -1;
        dq->rear = -1;
        dq->count = 0;
    }
}

bool isDequeEmpty(Deque *dq) {
    return (dq == NULL || dq->count == 0);
}

bool isDequeFull(Deque *dq) {
    return (dq != NULL && dq->count >= DEQUE_CAPACITY);
}

bool insertFrontDeque(Deque *dq, DequeItem item) {
    if (!dq || isDequeFull(dq)) return false;
    if (dq->front == -1) {
        dq->front = 0;
        dq->rear = 0;
    } else {
        dq->front = (dq->front - 1 + DEQUE_CAPACITY) % DEQUE_CAPACITY;
    }
    dq->items[dq->front] = item;
    dq->count++;
    return true;
}

bool insertRearDeque(Deque *dq, DequeItem item) {
    if (!dq || isDequeFull(dq)) return false;
    if (dq->front == -1) {
        dq->front = 0;
        dq->rear = 0;
    } else {
        dq->rear = (dq->rear + 1) % DEQUE_CAPACITY;
    }
    dq->items[dq->rear] = item;
    dq->count++;
    return true;
}

bool deleteFrontDeque(Deque *dq, DequeItem *outItem) {
    if (!dq || isDequeEmpty(dq)) return false;
    if (outItem) *outItem = dq->items[dq->front];
    if (dq->front == dq->rear) {
        dq->front = -1;
        dq->rear = -1;
    } else {
        dq->front = (dq->front + 1) % DEQUE_CAPACITY;
    }
    dq->count--;
    return true;
}

bool deleteRearDeque(Deque *dq, DequeItem *outItem) {
    if (!dq || isDequeEmpty(dq)) return false;
    if (outItem) *outItem = dq->items[dq->rear];
    if (dq->front == dq->rear) {
        dq->front = -1;
        dq->rear = -1;
    } else {
        dq->rear = (dq->rear - 1 + DEQUE_CAPACITY) % DEQUE_CAPACITY;
    }
    dq->count--;
    return true;
}

char* serializeDequeJSON(Deque *dq) {
    static char jsonBuf[2048];
    jsonBuf[0] = '\0';
    strcat(jsonBuf, "[");
    if (dq && dq->count > 0) {
        int idx = dq->front;
        for (int i = 0; i < dq->count; i++) {
            char itemBuf[256];
            snprintf(itemBuf, sizeof(itemBuf),
                     "{\"index\":%d,\"priority\":%d,\"description\":\"%s\"}",
                     idx, dq->items[idx].priority, dq->items[idx].description);
            strcat(jsonBuf, itemBuf);
            if (i < dq->count - 1) strcat(jsonBuf, ",");
            idx = (idx + 1) % DEQUE_CAPACITY;
        }
    }
    strcat(jsonBuf, "]");
    return jsonBuf;
}
