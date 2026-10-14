#ifndef JSON_H
#define JSON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* readEntireFile(const char *filepath);
int jsonGetInt(const char *json, const char *key, int defaultVal);
double jsonGetDouble(const char *json, const char *key, double defaultVal);
void jsonGetString(const char *json, const char *key, char *outBuf, int maxLen);

#endif // JSON_H
