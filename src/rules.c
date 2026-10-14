#include "rules.h"
#include "expression.h"

void initRuleSet(RuleSet *rs) {
    if (rs) rs->count = 0;
}

void addRule(RuleSet *rs, Rule r) {
    if (!rs || rs->count >= 20) return;
    rs->rules[rs->count++] = r;
}

RuleEvalResult evaluateRules(RuleSet *rs, ConfigLinkedList *config) {
    RuleEvalResult res;
    res.isValid = true;
    res.violationMessage[0] = '\0';
    res.requiredOption[0] = '\0';
    res.hasSurcharge = false;

    if (!rs || !config) return res;

    // Check configuration options
    bool hasMotor15HP = (searchNode(config, 203) != NULL);
    bool hasVoltage415 = (searchNode(config, 205) != NULL);
    bool hasVoltage230 = (searchNode(config, 204) != NULL);
    bool hasSensor = (searchNode(config, 210) != NULL || searchNode(config, 211) != NULL);
    bool hasController = (searchNode(config, 208) != NULL || searchNode(config, 209) != NULL);
    bool hasAdvController = (searchNode(config, 209) != NULL);
    bool hasMountingKit = (searchNode(config, 212) != NULL);
    bool hasSSBody = (searchNode(config, 207) != NULL);

    // Rule 1: Motor 15HP -> 415V required
    if (hasMotor15HP && !hasVoltage415) {
        res.isValid = true;
        strcpy(res.requiredOption, "415 V Power");
    }

    // Rule 2: Sensor -> Controller required
    if (hasSensor && !hasController) {
        res.isValid = true;
        strcpy(res.requiredOption, "Advanced Controller");
    }

    // Rule 3: Stainless Steel -> Premium surcharge
    if (hasSSBody) {
        res.hasSurcharge = true;
    }

    // Rule 4: Advanced Controller -> Mounting Kit required
    if (hasAdvController && !hasMountingKit) {
        res.isValid = true;
        strcpy(res.requiredOption, "Mounting Hardware Kit");
    }

    // Rule 5: 15 HP Motor + 230 V = INVALID
    if (hasMotor15HP && hasVoltage230) {
        res.isValid = false;
        strcpy(res.violationMessage, "CRITICAL ERROR: 15 HP Motor cannot run on 230 V Power supply! (Incompatible combination)");
    }

    return res;
}

char* serializeRuleSetJSON(RuleSet *rs) {
    static char jsonBuf[4096];
    jsonBuf[0] = '\0';
    strcat(jsonBuf, "[");
    if (rs) {
        for (int i = 0; i < rs->count; i++) {
            char rBuf[512];
            snprintf(rBuf, sizeof(rBuf),
                     "{\"ruleId\":%d,\"name\":\"%s\",\"condition\":\"%s\",\"action\":\"%s\",\"targetOption\":\"%s\",\"description\":\"%s\"}",
                     rs->rules[i].ruleId, rs->rules[i].name, rs->rules[i].condition, rs->rules[i].action, rs->rules[i].targetOption, rs->rules[i].description);
            strcat(jsonBuf, rBuf);
            if (i < rs->count - 1) strcat(jsonBuf, ",");
        }
    }
    strcat(jsonBuf, "]");
    return jsonBuf;
}
