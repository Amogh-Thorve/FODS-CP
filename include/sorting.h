#ifndef SORTING_H
#define SORTING_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
    char algorithmName[32];
    int datasetSize;
    long comparisons;
    long swaps;
    double executionTimeMs;
    int *sortedArray;
} SortResult;

SortResult bubbleSort(int arr[], int n);
SortResult selectionSort(int arr[], int n);
SortResult insertionSort(int arr[], int n);
SortResult mergeSort(int arr[], int n);
SortResult quickSort(int arr[], int n);
SortResult heapSort(int arr[], int n);
SortResult radixSort(int arr[], int n);
SortResult shellSort(int arr[], int n);
SortResult countingSort(int arr[], int n);
SortResult bucketSort(int arr[], int n);

char* serializeSortResultJSON(SortResult res);
void freeSortResult(SortResult *res);

#endif // SORTING_H
