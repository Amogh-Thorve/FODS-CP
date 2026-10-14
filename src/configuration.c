#include "configuration.h"
#include "cycle_detection.h"

void initCPQEngine(CPQEngine *engine) {
    if (!engine) return;
    initProductArray(&engine->products);
    initOptionArray(&engine->options);
    initLinkedList(&engine->currentConfig);
    initHistoryList(&engine->history);
    initGraph(&engine->compatibilityGraph);
    initRuleSet(&engine->ruleSet);

    engine->taxRate = 0.18;
    engine->discountRate = 0.05;
    engine->selectedProductId = -1;
}

bool selectProduct(CPQEngine *engine, int productId) {
    if (!engine) return false;
    Product *p = findProductById(&engine->products, productId);
    if (!p) return false;

    clearList(&engine->currentConfig);
    insertEnd(&engine->currentConfig, p->productId, p->name, p->category, p->basePrice, 0);
    engine->selectedProductId = productId;

    PriceBreakdown pb = calculatePrice(&engine->currentConfig, engine->taxRate, engine->discountRate);
    insertHistory(&engine->history, "SELECT_PRODUCT", p->productId, p->name, pb.finalTotalPrice);
    return true;
}

bool selectOption(CPQEngine *engine, int optionId) {
    if (!engine) return false;
    Option *opt = findOptionById(&engine->options, optionId);
    if (!opt) return false;

    // Check if already selected
    if (searchNode(&engine->currentConfig, optionId) != NULL) return true;

    insertEnd(&engine->currentConfig, opt->optionId, opt->name, opt->category, opt->price, 1);
    PriceBreakdown pb = calculatePrice(&engine->currentConfig, engine->taxRate, engine->discountRate);
    insertHistory(&engine->history, "ADD_OPTION", opt->optionId, opt->name, pb.finalTotalPrice);
    return true;
}

bool removeOption(CPQEngine *engine, int optionId) {
    if (!engine) return false;
    ConfigNode *node = searchNode(&engine->currentConfig, optionId);
    if (!node || !node->isOption) return false;

    char nameCopy[100];
    strcpy(nameCopy, node->name);
    deleteByValue(&engine->currentConfig, optionId);

    PriceBreakdown pb = calculatePrice(&engine->currentConfig, engine->taxRate, engine->discountRate);
    insertHistory(&engine->history, "REMOVE_OPTION", optionId, nameCopy, pb.finalTotalPrice);
    return true;
}

bool undoConfiguration(CPQEngine *engine) {
    if (!engine) return false;
    HistoryNode *prev = previousState(&engine->history);
    return (prev != NULL);
}

bool redoConfiguration(CPQEngine *engine) {
    if (!engine) return false;
    HistoryNode *next = nextState(&engine->history);
    return (next != NULL);
}

ValidationResult validateCurrentConfiguration(CPQEngine *engine) {
    ValidationResult vr;
    memset(&vr, 0, sizeof(vr));
    vr.isValid = true;
    strcpy(vr.status, "VALID");
    strcpy(vr.details, "Configuration meets all product specifications and compatibility rules.");

    if (!engine || engine->currentConfig.count == 0) {
        vr.isValid = false;
        strcpy(vr.status, "INVALID");
        strcpy(vr.details, "No base product selected.");
        return vr;
    }

    // Graph Cycle Detection
    CycleResult cyc = detectCycle(&engine->compatibilityGraph);
    if (cyc.hasCycle) {
        vr.isValid = false;
        strcpy(vr.status, "INVALID");
        strcpy(vr.details, "Circular dependency detected in graph rules!");
        return vr;
    }

    // Rule Evaluation
    RuleEvalResult rEval = evaluateRules(&engine->ruleSet, &engine->currentConfig);
    if (!rEval.isValid) {
        vr.isValid = false;
        strcpy(vr.status, "INVALID");
        strncpy(vr.details, rEval.violationMessage, sizeof(vr.details) - 1);
        return vr;
    }

    if (strlen(rEval.requiredOption) > 0) {
        snprintf(vr.details, sizeof(vr.details), "VALID - Required companion option identified: '%s'.", rEval.requiredOption);
    }

    vr.pricing = calculatePrice(&engine->currentConfig, engine->taxRate, engine->discountRate);
    return vr;
}

char* serializeValidationJSON(ValidationResult vr) {
    static char jsonBuf[2048];
    char priceJson[512];
    strcpy(priceJson, serializePriceJSON(vr.pricing));
    snprintf(jsonBuf, sizeof(jsonBuf),
             "{\"isValid\":%s,\"status\":\"%s\",\"details\":\"%s\",\"pricing\":%s}",
             vr.isValid ? "true" : "false", vr.status, vr.details, priceJson);
    return jsonBuf;
}
