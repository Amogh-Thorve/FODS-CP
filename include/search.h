#ifndef SEARCH_H
#define SEARCH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
    char algorithmName[32];
    int targetId;
    int foundIndex;
    int comparisons;
    double executionTimeMs;
} SearchResult;

SearchResult linearSearch(int arr[], int n, int key);
SearchResult binarySearch(int arr[], int n, int key);
SearchResult indexedSequentialSearch(int arr[], int n, int key);
SearchResult interpolationSearch(int arr[], int n, int key);
SearchResult fibonacciSearch(int arr[], int n, int key);

char* serializeSearchResultJSON(SearchResult res);

#endif // SEARCH_H
