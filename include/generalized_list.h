#ifndef GENERALIZED_LIST_H
#define GENERALIZED_LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum { ATOM, LIST } GNodeType;

typedef struct GListNode {
    GNodeType type;
    union {
        struct {
            int id;
            char name[64];
            double price;
        } atom;
        struct {
            char categoryName[64];
            struct GListNode *subList;
        } list;
    } data;
    struct GListNode *next;
} GListNode;

GListNode* createAtomNode(int id, const char *name, double price);
GListNode* createListNode(const char *categoryName, GListNode *subList);
void appendGNode(GListNode **head, GListNode *newNode);
void freeGList(GListNode *head);
char* serializeGListJSON(GListNode *head);

#endif // GENERALIZED_LIST_H
