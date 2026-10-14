#include "json.h"

char* readEntireFile(const char *filepath) {
    FILE *f = fopen(filepath, "rb");
    if (!f) return NULL;
    fseek(f, 0, SEEK_END);
    long sz = ftell(f);
    fseek(f, 0, SEEK_SET);

    char *buf = (char*)malloc(sz + 1);
    if (!buf) {
        fclose(f);
        return NULL;
    }
    size_t readSz = fread(buf, 1, sz, f);
    buf[readSz] = '\0';
    fclose(f);
    return buf;
}

int jsonGetInt(const char *json, const char *key, int defaultVal) {
    if (!json || !key) return defaultVal;
    char searchKey[64];
    snprintf(searchKey, sizeof(searchKey), "\"%s\":", key);
    char *p = strstr(json, searchKey);
    if (!p) {
        snprintf(searchKey, sizeof(searchKey), "\"%s\" :", key);
        p = strstr(json, searchKey);
    }
    if (!p) return defaultVal;
    p = strchr(p, ':');
    if (!p) return defaultVal;
    return atoi(p + 1);
}

double jsonGetDouble(const char *json, const char *key, double defaultVal) {
    if (!json || !key) return defaultVal;
    char searchKey[64];
    snprintf(searchKey, sizeof(searchKey), "\"%s\":", key);
    char *p = strstr(json, searchKey);
    if (!p) {
        snprintf(searchKey, sizeof(searchKey), "\"%s\" :", key);
        p = strstr(json, searchKey);
    }
    if (!p) return defaultVal;
    p = strchr(p, ':');
    if (!p) return defaultVal;
    return atof(p + 1);
}

void jsonGetString(const char *json, const char *key, char *outBuf, int maxLen) {
    if (!outBuf || maxLen <= 0) return;
    outBuf[0] = '\0';
    if (!json || !key) return;

    char searchKey[64];
    snprintf(searchKey, sizeof(searchKey), "\"%s\":", key);
    char *p = strstr(json, searchKey);
    if (!p) {
        snprintf(searchKey, sizeof(searchKey), "\"%s\" :", key);
        p = strstr(json, searchKey);
    }
    if (!p) return;
    p = strchr(p, '"');
    if (!p) return;
    p = strchr(p + 1, '"');
    if (!p) return;
    p++; // move past '"'
    while (*p == ' ' || *p == '"') p++;

    int idx = 0;
    while (*p && *p != '"' && *p != ',' && *p != '}' && idx < maxLen - 1) {
        outBuf[idx++] = *p++;
    }
    outBuf[idx] = '\0';
}
