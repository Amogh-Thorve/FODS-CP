#include "hashing.h"

static unsigned int hashFunction(const char *key) {
    unsigned int hash = 0;
    while (*key) {
        hash = (hash * 31) + (unsigned char)(*key++);
    }
    return hash % TABLE_SIZE;
}

// Separate Chaining Implementation
void initChainingHashTable(ChainingHashTable *ht) {
    if (!ht) return;
    ht->itemCount = 0;
    for (int i = 0; i < TABLE_SIZE; i++) {
        ht->table[i] = NULL;
    }
}

void insertChaining(ChainingHashTable *ht, const char *key, Product p) {
    if (!ht || !key) return;
    unsigned int idx = hashFunction(key);
    HashChainNode *node = (HashChainNode*)malloc(sizeof(HashChainNode));
    if (!node) return;
    strncpy(node->key, key, sizeof(node->key) - 1);
    node->key[sizeof(node->key) - 1] = '\0';
    node->product = p;
    node->next = ht->table[idx];
    ht->table[idx] = node;
    ht->itemCount++;
}

Product* searchChaining(ChainingHashTable *ht, const char *key) {
    if (!ht || !key) return NULL;
    unsigned int idx = hashFunction(key);
    HashChainNode *curr = ht->table[idx];
    while (curr) {
        if (strcmp(curr->key, key) == 0) {
            return &curr->product;
        }
        curr = curr->next;
    }
    return NULL;
}

bool deleteChaining(ChainingHashTable *ht, const char *key) {
    if (!ht || !key) return false;
    unsigned int idx = hashFunction(key);
    HashChainNode *curr = ht->table[idx];
    HashChainNode *prev = NULL;
    while (curr) {
        if (strcmp(curr->key, key) == 0) {
            if (prev) prev->next = curr->next;
            else ht->table[idx] = curr->next;
            free(curr);
            ht->itemCount--;
            return true;
        }
        prev = curr;
        curr = curr->next;
    }
    return false;
}

void freeChainingHashTable(ChainingHashTable *ht) {
    if (!ht) return;
    for (int i = 0; i < TABLE_SIZE; i++) {
        HashChainNode *curr = ht->table[i];
        while (curr) {
            HashChainNode *temp = curr;
            curr = curr->next;
            free(temp);
        }
        ht->table[i] = NULL;
    }
    ht->itemCount = 0;
}

char* serializeChainingHashTableJSON(ChainingHashTable *ht) {
    static char jsonBuf[8192];
    jsonBuf[0] = '\0';
    strcat(jsonBuf, "[");
    for (int i = 0; i < TABLE_SIZE; i++) {
        char bucketBuf[512];
        snprintf(bucketBuf, sizeof(bucketBuf), "{\"index\":%d,\"chain\":[", i);
        strcat(jsonBuf, bucketBuf);
        HashChainNode *curr = ht->table[i];
        int first = 1;
        while (curr) {
            if (!first) strcat(jsonBuf, ",");
            first = 0;
            char nBuf[256];
            snprintf(nBuf, sizeof(nBuf), "{\"key\":\"%s\",\"name\":\"%s\"}", curr->key, curr->product.name);
            strcat(jsonBuf, nBuf);
            curr = curr->next;
        }
        strcat(jsonBuf, "]}");
        if (i < TABLE_SIZE - 1) strcat(jsonBuf, ",");
    }
    strcat(jsonBuf, "]");
    return jsonBuf;
}

// Open Addressing Implementation
void initOpenAddressingHashTable(OpenAddressingHashTable *ht) {
    if (!ht) return;
    ht->itemCount = 0;
    ht->collisions = 0;
    for (int i = 0; i < TABLE_SIZE; i++) {
        ht->table[i].state = EMPTY;
        ht->table[i].key[0] = '\0';
    }
}

bool insertLinearProbing(OpenAddressingHashTable *ht, const char *key, Product p) {
    if (!ht || !key || ht->itemCount >= TABLE_SIZE) return false;
    unsigned int hashIdx = hashFunction(key);
    for (int i = 0; i < TABLE_SIZE; i++) {
        int idx = (hashIdx + i) % TABLE_SIZE;
        if (ht->table[idx].state == EMPTY || ht->table[idx].state == DELETED) {
            strncpy(ht->table[idx].key, key, sizeof(ht->table[idx].key) - 1);
            ht->table[idx].key[sizeof(ht->table[idx].key) - 1] = '\0';
            ht->table[idx].product = p;
            ht->table[idx].state = OCCUPIED;
            ht->itemCount++;
            return true;
        }
        ht->collisions++;
    }
    return false;
}

bool insertQuadraticProbing(OpenAddressingHashTable *ht, const char *key, Product p) {
    if (!ht || !key || ht->itemCount >= TABLE_SIZE) return false;
    unsigned int hashIdx = hashFunction(key);
    for (int i = 0; i < TABLE_SIZE; i++) {
        int idx = (hashIdx + i * i) % TABLE_SIZE;
        if (ht->table[idx].state == EMPTY || ht->table[idx].state == DELETED) {
            strncpy(ht->table[idx].key, key, sizeof(ht->table[idx].key) - 1);
            ht->table[idx].key[sizeof(ht->table[idx].key) - 1] = '\0';
            ht->table[idx].product = p;
            ht->table[idx].state = OCCUPIED;
            ht->itemCount++;
            return true;
        }
        ht->collisions++;
    }
    return false;
}

Product* searchOpenAddressing(OpenAddressingHashTable *ht, const char *key, bool isQuadratic) {
    if (!ht || !key) return NULL;
    unsigned int hashIdx = hashFunction(key);
    for (int i = 0; i < TABLE_SIZE; i++) {
        int idx = isQuadratic ? (hashIdx + i * i) % TABLE_SIZE : (hashIdx + i) % TABLE_SIZE;
        if (ht->table[idx].state == EMPTY) return NULL;
        if (ht->table[idx].state == OCCUPIED && strcmp(ht->table[idx].key, key) == 0) {
            return &ht->table[idx].product;
        }
    }
    return NULL;
}

char* serializeOpenAddressingHashTableJSON(OpenAddressingHashTable *ht) {
    static char jsonBuf[8192];
    jsonBuf[0] = '\0';
    snprintf(jsonBuf, sizeof(jsonBuf), "{\"itemCount\":%d,\"collisions\":%d,\"slots\":[", ht->itemCount, ht->collisions);
    for (int i = 0; i < TABLE_SIZE; i++) {
        char slotBuf[256];
        const char *st = (ht->table[i].state == OCCUPIED) ? "OCCUPIED" : ((ht->table[i].state == DELETED) ? "DELETED" : "EMPTY");
        snprintf(slotBuf, sizeof(slotBuf),
                 "{\"index\":%d,\"state\":\"%s\",\"key\":\"%s\",\"name\":\"%s\"}",
                 i, st, ht->table[i].key, (ht->table[i].state == OCCUPIED) ? ht->table[i].product.name : "");
        strcat(jsonBuf, slotBuf);
        if (i < TABLE_SIZE - 1) strcat(jsonBuf, ",");
    }
    strcat(jsonBuf, "]}");
    return jsonBuf;
}
