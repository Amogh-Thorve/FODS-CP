#ifndef RULES_H
#define RULES_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "linked_list.h"

typedef struct {
    int ruleId;
    char name[64];
    char condition[128];
    char action[32]; // "REQUIRE", "INVALID", "SURCHARGE"
    char targetOption[64];
    char description[256];
} Rule;

typedef struct {
    Rule rules[20];
    int count;
} RuleSet;

typedef struct {
    bool isValid;
    char violationMessage[256];
    char requiredOption[64];
    bool hasSurcharge;
} RuleEvalResult;

void initRuleSet(RuleSet *rs);
void addRule(RuleSet *rs, Rule r);
RuleEvalResult evaluateRules(RuleSet *rs, ConfigLinkedList *config);
char* serializeRuleSetJSON(RuleSet *rs);

#endif // RULES_H
