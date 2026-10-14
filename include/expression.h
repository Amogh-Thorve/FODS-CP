#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char stepName[64];
    char symbolProcessed[32];
    char stackContents[256];
    char outputExpression[256];
} ConversionStep;

typedef struct {
    char originalInfix[256];
    char convertedExpression[256];
    char conversionType[16]; // "POSTFIX" or "PREFIX"
    int evaluationResult;
    ConversionStep steps[50];
    int stepCount;
} ExpressionResult;

ExpressionResult infixToPostfix(const char *infix);
ExpressionResult infixToPrefix(const char *infix);
int evaluatePostfix(const char *postfix);
int evaluatePrefix(const char *prefix);
char* serializeExpressionResultJSON(ExpressionResult res);

#endif // EXPRESSION_H
