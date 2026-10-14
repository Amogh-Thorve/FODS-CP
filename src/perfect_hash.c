#include "perfect_hash.h"

// Minimal perfect hashing for static set of 8 CPQ material/voltage codes
static const struct {
    const char *key;
    const char *name;
    int hashVal;
} STATIC_CODES[8] = {
    {"V230", "230 V Power", 0},
    {"V415", "415 V Power", 1},
    {"M_CI", "Cast Iron Body", 2},
    {"M_SS", "Stainless Steel Body", 3},
    {"M_05", "5 HP Motor", 4},
    {"M_10", "10 HP Motor", 5},
    {"M_15", "15 HP Motor", 6},
    {"C_ADV", "Advanced Controller", 7}
};

PerfectHashTable buildPerfectHashTable(void) {
    PerfectHashTable ht;
    ht.count = 8;
    for (int i = 0; i < 8; i++) {
        strncpy(ht.slots[i].key, STATIC_CODES[i].key, sizeof(ht.slots[i].key) - 1);
        strncpy(ht.slots[i].name, STATIC_CODES[i].name, sizeof(ht.slots[i].name) - 1);
        ht.slots[i].hashVal = STATIC_CODES[i].hashVal;
    }
    return ht;
}

char* serializePerfectHashJSON(PerfectHashTable *ht) {
    static char jsonBuf[2048];
    jsonBuf[0] = '\0';
    strcat(jsonBuf, "[");
    for (int i = 0; i < ht->count; i++) {
        char sBuf[256];
        snprintf(sBuf, sizeof(sBuf),
                 "{\"index\":%d,\"key\":\"%s\",\"name\":\"%s\",\"collisions\":0}",
                 i, ht->slots[i].key, ht->slots[i].name);
        strcat(jsonBuf, sBuf);
        if (i < ht->count - 1) strcat(jsonBuf, ",");
    }
    strcat(jsonBuf, "]");
    return jsonBuf;
}
