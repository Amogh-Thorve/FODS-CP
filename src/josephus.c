#include "josephus.h"

typedef struct JNode {
    int id;
    struct JNode *next;
} JNode;

JosephusResult solveJosephus(int n, int k) {
    JosephusResult res;
    res.totalCount = n;
    res.stepSize = k;
    res.eliminatedCount = 0;
    res.survivorId = -1;

    if (n <= 0 || k <= 0) return res;

    // Create circular linked list
    JNode *head = (JNode*)malloc(sizeof(JNode));
    head->id = 1;
    JNode *curr = head;
    for (int i = 2; i <= n; i++) {
        JNode *newNode = (JNode*)malloc(sizeof(JNode));
        newNode->id = i;
        curr->next = newNode;
        curr = newNode;
    }
    curr->next = head; // circle complete

    JNode *prev = curr;
    curr = head;

    while (curr->next != curr) {
        // Count k-1 steps
        for (int i = 1; i < k; i++) {
            prev = curr;
            curr = curr->next;
        }
        // Eliminate curr node
        res.eliminationOrder[res.eliminatedCount++] = curr->id;
        prev->next = curr->next;
        free(curr);
        curr = prev->next;
    }

    res.survivorId = curr->id;
    free(curr);

    return res;
}

char* serializeJosephusJSON(JosephusResult res) {
    static char jsonBuf[2048];
    jsonBuf[0] = '\0';
    char elimBuf[1024];
    elimBuf[0] = '\0';
    strcat(elimBuf, "[");
    for (int i = 0; i < res.eliminatedCount; i++) {
        char val[16];
        snprintf(val, sizeof(val), "%d", res.eliminationOrder[i]);
        strcat(elimBuf, val);
        if (i < res.eliminatedCount - 1) strcat(elimBuf, ",");
    }
    strcat(elimBuf, "]");

    snprintf(jsonBuf, sizeof(jsonBuf),
             "{\"totalCount\":%d,\"stepSize\":%d,\"survivorId\":%d,\"eliminationOrder\":%s}",
             res.totalCount, res.stepSize, res.survivorId, elimBuf);
    return jsonBuf;
}
