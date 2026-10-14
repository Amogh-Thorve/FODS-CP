#ifndef HASHING_H
#define HASHING_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "arrays.h"

#define TABLE_SIZE 37

// Separate Chaining
typedef struct HashChainNode {
    char key[32];
    Product product;
    struct HashChainNode *next;
} HashChainNode;

typedef struct {
    HashChainNode* table[TABLE_SIZE];
    int itemCount;
} ChainingHashTable;

void initChainingHashTable(ChainingHashTable *ht);
void insertChaining(ChainingHashTable *ht, const char *key, Product p);
Product* searchChaining(ChainingHashTable *ht, const char *key);
bool deleteChaining(ChainingHashTable *ht, const char *key);
void freeChainingHashTable(ChainingHashTable *ht);
char* serializeChainingHashTableJSON(ChainingHashTable *ht);

// Open Addressing (Linear & Quadratic Probing)
typedef enum { EMPTY, OCCUPIED, DELETED } SlotState;

typedef struct {
    char key[32];
    Product product;
    SlotState state;
} HashSlot;

typedef struct {
    HashSlot table[TABLE_SIZE];
    int itemCount;
    int collisions;
} OpenAddressingHashTable;

void initOpenAddressingHashTable(OpenAddressingHashTable *ht);
bool insertLinearProbing(OpenAddressingHashTable *ht, const char *key, Product p);
bool insertQuadraticProbing(OpenAddressingHashTable *ht, const char *key, Product p);
Product* searchOpenAddressing(OpenAddressingHashTable *ht, const char *key, bool isQuadratic);
char* serializeOpenAddressingHashTableJSON(OpenAddressingHashTable *ht);

#endif // HASHING_H
