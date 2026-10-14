#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_STACK_SIZE 100

// Array-based Stack
typedef struct {
    char items[MAX_STACK_SIZE][64];
    int top;
} ArrayStack;

void initArrayStack(ArrayStack *s);
bool isArrayStackEmpty(ArrayStack *s);
bool isArrayStackFull(ArrayStack *s);
void pushArrayStack(ArrayStack *s, const char *val);
char* popArrayStack(ArrayStack *s, char *outVal);
char* peekArrayStack(ArrayStack *s);

// Linked List-based Stack
typedef struct StackNode {
    char data[64];
    struct StackNode *next;
} StackNode;

typedef struct {
    StackNode *top;
    int size;
} ListStack;

void initListStack(ListStack *s);
bool isListStackEmpty(ListStack *s);
void pushListStack(ListStack *s, const char *val);
char* popListStack(ListStack *s, char *outVal);
char* peekListStack(ListStack *s);
void freeListStack(ListStack *s);

#endif // STACK_H
