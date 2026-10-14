#include "stack.h"

// Array Stack Implementation
void initArrayStack(ArrayStack *s) {
    if (s) s->top = -1;
}

bool isArrayStackEmpty(ArrayStack *s) {
    return (s == NULL || s->top == -1);
}

bool isArrayStackFull(ArrayStack *s) {
    return (s != NULL && s->top >= MAX_STACK_SIZE - 1);
}

void pushArrayStack(ArrayStack *s, const char *val) {
    if (!s || isArrayStackFull(s)) return;
    s->top++;
    strncpy(s->items[s->top], val ? val : "", sizeof(s->items[s->top]) - 1);
    s->items[s->top][sizeof(s->items[s->top]) - 1] = '\0';
}

char* popArrayStack(ArrayStack *s, char *outVal) {
    if (!s || isArrayStackEmpty(s)) {
        if (outVal) outVal[0] = '\0';
        return NULL;
    }
    if (outVal) {
        strcpy(outVal, s->items[s->top]);
    }
    s->top--;
    return outVal;
}

char* peekArrayStack(ArrayStack *s) {
    if (!s || isArrayStackEmpty(s)) return NULL;
    return s->items[s->top];
}

// List Stack Implementation
void initListStack(ListStack *s) {
    if (s) {
        s->top = NULL;
        s->size = 0;
    }
}

bool isListStackEmpty(ListStack *s) {
    return (s == NULL || s->top == NULL);
}

void pushListStack(ListStack *s, const char *val) {
    if (!s) return;
    StackNode *node = (StackNode*)malloc(sizeof(StackNode));
    if (!node) return;
    strncpy(node->data, val ? val : "", sizeof(node->data) - 1);
    node->data[sizeof(node->data) - 1] = '\0';
    node->next = s->top;
    s->top = node;
    s->size++;
}

char* popListStack(ListStack *s, char *outVal) {
    if (!s || isListStackEmpty(s)) {
        if (outVal) outVal[0] = '\0';
        return NULL;
    }
    StackNode *temp = s->top;
    if (outVal) strcpy(outVal, temp->data);
    s->top = temp->next;
    free(temp);
    s->size--;
    return outVal;
}

char* peekListStack(ListStack *s) {
    if (!s || isListStackEmpty(s)) return NULL;
    return s->top->data;
}

void freeListStack(ListStack *s) {
    if (!s) return;
    while (!isListStackEmpty(s)) {
        popListStack(s, NULL);
    }
}
