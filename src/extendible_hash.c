#include "extendible_hash.h"

ExtendibleHashTable* createExtendibleHash(int initialGlobalDepth) {
    ExtendibleHashTable *eh = (ExtendibleHashTable*)malloc(sizeof(ExtendibleHashTable));
    if (!eh) return NULL;
    eh->globalDepth = initialGlobalDepth;
    eh->dirSize = 1 << initialGlobalDepth;
    eh->dir = (ExtBucket**)malloc(eh->dirSize * sizeof(ExtBucket*));

    ExtBucket *b0 = (ExtBucket*)malloc(sizeof(ExtBucket));
    b0->localDepth = initialGlobalDepth;
    b0->count = 0;

    for (int i = 0; i < eh->dirSize; i++) {
        eh->dir[i] = b0;
    }
    return eh;
}

static int getHash(int key, int depth) {
    return key & ((1 << depth) - 1);
}

void insertExtendibleHash(ExtendibleHashTable *eh, int key) {
    if (!eh) return;
    int idx = getHash(key, eh->globalDepth);
    ExtBucket *b = eh->dir[idx];

    if (b->count < BUCKET_CAPACITY) {
        b->keys[b->count++] = key;
        return;
    }

    // Split Bucket
    if (b->localDepth == eh->globalDepth) {
        // Double directory
        int oldSize = eh->dirSize;
        eh->globalDepth++;
        eh->dirSize = 1 << eh->globalDepth;
        eh->dir = (ExtBucket**)realloc(eh->dir, eh->dirSize * sizeof(ExtBucket*));
        for (int i = oldSize; i < eh->dirSize; i++) {
            eh->dir[i] = eh->dir[i - oldSize];
        }
    }

    b->localDepth++;
    ExtBucket *newBucket = (ExtBucket*)malloc(sizeof(ExtBucket));
    newBucket->localDepth = b->localDepth;
    newBucket->count = 0;

    int tempKeys[BUCKET_CAPACITY + 1];
    for (int i = 0; i < BUCKET_CAPACITY; i++) tempKeys[i] = b->keys[i];
    tempKeys[BUCKET_CAPACITY] = key;
    b->count = 0;

    // Update directory pointers
    int mask = 1 << (b->localDepth - 1);
    for (int i = 0; i < eh->dirSize; i++) {
        if (eh->dir[i] == b) {
            if (i & mask) {
                eh->dir[i] = newBucket;
            }
        }
    }

    // Rehash keys
    for (int i = 0; i <= BUCKET_CAPACITY; i++) {
        insertExtendibleHash(eh, tempKeys[i]);
    }
}

void freeExtendibleHash(ExtendibleHashTable *eh) {
    if (!eh) return;
    // Basic clean up
    free(eh->dir);
    free(eh);
}

char* serializeExtendibleHashJSON(ExtendibleHashTable *eh) {
    static char jsonBuf[4096];
    jsonBuf[0] = '\0';
    if (!eh) return "{}";

    snprintf(jsonBuf, sizeof(jsonBuf),
             "{\"globalDepth\":%d,\"dirSize\":%d,\"directory\":[",
             eh->globalDepth, eh->dirSize);

    for (int i = 0; i < eh->dirSize; i++) {
        char dirBuf[512];
        ExtBucket *b = eh->dir[i];
        snprintf(dirBuf, sizeof(dirBuf),
                 "{\"dirIndex\":%d,\"localDepth\":%d,\"bucketKeys\":[",
                 i, b->localDepth);
        strcat(jsonBuf, dirBuf);
        for (int k = 0; k < b->count; k++) {
            char kStr[16];
            snprintf(kStr, sizeof(kStr), "%d", b->keys[k]);
            strcat(jsonBuf, kStr);
            if (k < b->count - 1) strcat(jsonBuf, ",");
        }
        strcat(jsonBuf, "]}");
        if (i < eh->dirSize - 1) strcat(jsonBuf, ",");
    }
    strcat(jsonBuf, "]}");
    return jsonBuf;
}
