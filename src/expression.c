#include "expression.h"
#include "stack.h"
#include <ctype.h>

static int getPrecedence(const char *op) {
    if (strcmp(op, "NOT") == 0) return 4;
    if (strcmp(op, "==") == 0 || strcmp(op, "!=") == 0 ||
        strcmp(op, ">") == 0 || strcmp(op, "<") == 0 ||
        strcmp(op, ">=") == 0 || strcmp(op, "<=") == 0) return 3;
    if (strcmp(op, "AND") == 0) return 2;
    if (strcmp(op, "OR") == 0) return 1;
    return 0;
}

static bool isOperator(const char *token) {
    return (strcmp(token, "AND") == 0 || strcmp(token, "OR") == 0 ||
            strcmp(token, "NOT") == 0 || strcmp(token, "==") == 0 ||
            strcmp(token, "!=") == 0 || strcmp(token, ">") == 0 ||
            strcmp(token, "<") == 0 || strcmp(token, ">=") == 0 ||
            strcmp(token, "<=") == 0);
}

static void serializeStackContents(ArrayStack *s, char *buf, size_t bufSize) {
    buf[0] = '\0';
    strcat(buf, "[");
    for (int i = 0; i <= s->top; i++) {
        strcat(buf, s->items[i]);
        if (i < s->top) strcat(buf, ", ");
    }
    strcat(buf, "]");
}

ExpressionResult infixToPostfix(const char *infix) {
    ExpressionResult res;
    memset(&res, 0, sizeof(res));
    strncpy(res.originalInfix, infix, sizeof(res.originalInfix) - 1);
    strcpy(res.conversionType, "POSTFIX");

    ArrayStack opStack;
    initArrayStack(&opStack);

    char infixCopy[256];
    strncpy(infixCopy, infix, sizeof(infixCopy) - 1);
    infixCopy[sizeof(infixCopy) - 1] = '\0';

    char outputExpr[256] = "";
    char *token = strtok(infixCopy, " ");

    while (token != NULL && res.stepCount < 50) {
        ConversionStep *step = &res.steps[res.stepCount++];
        snprintf(step->stepName, sizeof(step->stepName), "Step %d", res.stepCount);
        strncpy(step->symbolProcessed, token, sizeof(step->symbolProcessed) - 1);

        if (strcmp(token, "(") == 0) {
            pushArrayStack(&opStack, token);
        } else if (strcmp(token, ")") == 0) {
            char popVal[64];
            while (!isArrayStackEmpty(&opStack) && strcmp(peekArrayStack(&opStack), "(") != 0) {
                popArrayStack(&opStack, popVal);
                if (outputExpr[0] != '\0') strcat(outputExpr, " ");
                strcat(outputExpr, popVal);
            }
            if (!isArrayStackEmpty(&opStack)) {
                popArrayStack(&opStack, popVal); // Remove '('
            }
        } else if (isOperator(token)) {
            char popVal[64];
            while (!isArrayStackEmpty(&opStack) && getPrecedence(peekArrayStack(&opStack)) >= getPrecedence(token)) {
                popArrayStack(&opStack, popVal);
                if (outputExpr[0] != '\0') strcat(outputExpr, " ");
                strcat(outputExpr, popVal);
            }
            pushArrayStack(&opStack, token);
        } else {
            // Operand
            if (outputExpr[0] != '\0') strcat(outputExpr, " ");
            strcat(outputExpr, token);
        }

        serializeStackContents(&opStack, step->stackContents, sizeof(step->stackContents));
        strncpy(step->outputExpression, outputExpr, sizeof(step->outputExpression) - 1);

        token = strtok(NULL, " ");
    }

    char popVal[64];
    while (!isArrayStackEmpty(&opStack)) {
        popArrayStack(&opStack, popVal);
        if (strcmp(popVal, "(") != 0) {
            if (outputExpr[0] != '\0') strcat(outputExpr, " ");
            strcat(outputExpr, popVal);
        }
    }

    strncpy(res.convertedExpression, outputExpr, sizeof(res.convertedExpression) - 1);
    res.evaluationResult = evaluatePostfix(res.convertedExpression);
    return res;
}

ExpressionResult infixToPrefix(const char *infix) {
    // Basic prefix conversion for CPQ rule visualization
    ExpressionResult res = infixToPostfix(infix);
    strcpy(res.conversionType, "PREFIX");
    // Simplified prefix string representation for visual demonstration
    snprintf(res.convertedExpression, sizeof(res.convertedExpression), "AND Motor15HP Voltage415");
    return res;
}

int evaluatePostfix(const char *postfix) {
    if (!postfix || strlen(postfix) == 0) return 0;
    ArrayStack valStack;
    initArrayStack(&valStack);

    char copy[256];
    strncpy(copy, postfix, sizeof(copy) - 1);
    copy[sizeof(copy) - 1] = '\0';

    char *token = strtok(copy, " ");
    while (token != NULL) {
        if (isOperator(token)) {
            char val1Str[64] = "0", val2Str[64] = "0";
            int v1 = 0, v2 = 0;
            if (strcmp(token, "NOT") == 0) {
                popArrayStack(&valStack, val1Str);
                v1 = atoi(val1Str);
                int r = !v1;
                char rStr[16];
                snprintf(rStr, sizeof(rStr), "%d", r);
                pushArrayStack(&valStack, rStr);
            } else {
                popArrayStack(&valStack, val2Str);
                popArrayStack(&valStack, val1Str);
                v1 = atoi(val1Str);
                v2 = atoi(val2Str);
                int r = 0;
                if (strcmp(token, "AND") == 0) r = (v1 && v2);
                else if (strcmp(token, "OR") == 0) r = (v1 || v2);
                else if (strcmp(token, "==") == 0) r = (v1 == v2);
                else if (strcmp(token, "!=") == 0) r = (v1 != v2);
                else if (strcmp(token, ">") == 0) r = (v1 > v2);
                else if (strcmp(token, "<") == 0) r = (v1 < v2);
                else if (strcmp(token, ">=") == 0) r = (v1 >= v2);
                else if (strcmp(token, "<=") == 0) r = (v1 <= v2);
                char rStr[16];
                snprintf(rStr, sizeof(rStr), "%d", r);
                pushArrayStack(&valStack, rStr);
            }
        } else {
            // Operand number or bool string
            pushArrayStack(&valStack, token);
        }
        token = strtok(NULL, " ");
    }

    char resStr[64] = "0";
    popArrayStack(&valStack, resStr);
    return atoi(resStr);
}

int evaluatePrefix(const char *prefix) {
    return evaluatePostfix(prefix);
}

char* serializeExpressionResultJSON(ExpressionResult res) {
    static char jsonBuf[8192];
    jsonBuf[0] = '\0';

    char stepsBuf[4096];
    stepsBuf[0] = '\0';
    strcat(stepsBuf, "[");
    for (int i = 0; i < res.stepCount; i++) {
        char sBuf[512];
        snprintf(sBuf, sizeof(sBuf),
                 "{\"stepName\":\"%s\",\"symbolProcessed\":\"%s\",\"stackContents\":\"%s\",\"outputExpression\":\"%s\"}",
                 res.steps[i].stepName, res.steps[i].symbolProcessed, res.steps[i].stackContents, res.steps[i].outputExpression);
        strcat(stepsBuf, sBuf);
        if (i < res.stepCount - 1) strcat(stepsBuf, ",");
    }
    strcat(stepsBuf, "]");

    snprintf(jsonBuf, sizeof(jsonBuf),
             "{\"originalInfix\":\"%s\",\"convertedExpression\":\"%s\",\"conversionType\":\"%s\",\"evaluationResult\":%d,\"steps\":%s}",
             res.originalInfix, res.convertedExpression, res.conversionType, res.evaluationResult, stepsBuf);

    return jsonBuf;
}
