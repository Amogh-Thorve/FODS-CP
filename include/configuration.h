#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include "arrays.h"
#include "linked_list.h"
#include "doubly_list.h"
#include "graph.h"
#include "rules.h"
#include "pricing.h"

typedef struct {
    ProductArray products;
    OptionArray options;
    ConfigLinkedList currentConfig;
    DoublyHistoryList history;
    Graph compatibilityGraph;
    RuleSet ruleSet;
    double taxRate;
    double discountRate;
    int selectedProductId;
} CPQEngine;

void initCPQEngine(CPQEngine *engine);
bool selectProduct(CPQEngine *engine, int productId);
bool selectOption(CPQEngine *engine, int optionId);
bool removeOption(CPQEngine *engine, int optionId);
bool undoConfiguration(CPQEngine *engine);
bool redoConfiguration(CPQEngine *engine);

typedef struct {
    bool isValid;
    char status[16]; // "VALID" or "INVALID"
    char details[512];
    PriceBreakdown pricing;
} ValidationResult;

ValidationResult validateCurrentConfiguration(CPQEngine *engine);
char* serializeValidationJSON(ValidationResult vr);

#endif // CONFIGURATION_H
