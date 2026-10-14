#include "benchmark.h"

BenchmarkSuite runFullBenchmark(int datasetSize) {
    BenchmarkSuite suite;
    memset(&suite, 0, sizeof(suite));
    suite.datasetSize = datasetSize;

    int *arr = (int*)malloc(datasetSize * sizeof(int));
    if (!arr) return suite;

    // Generate test dataset
    for (int i = 0; i < datasetSize; i++) {
        arr[i] = i * 2 + 1;
    }

    int target = arr[datasetSize / 2];

    // Search Benchmarks
    suite.searchResults[0] = linearSearch(arr, datasetSize, target);
    suite.searchResults[1] = binarySearch(arr, datasetSize, target);
    suite.searchResults[2] = indexedSequentialSearch(arr, datasetSize, target);
    suite.searchResults[3] = interpolationSearch(arr, datasetSize, target);
    suite.searchResults[4] = fibonacciSearch(arr, datasetSize, target);
    suite.searchResultCount = 5;

    // Unsort array for sorting benchmark
    int *unsortedArr = (int*)malloc(datasetSize * sizeof(int));
    for (int i = 0; i < datasetSize; i++) {
        unsortedArr[i] = (datasetSize - i) * 3 % (datasetSize * 2 + 1);
    }

    // Sort Benchmarks
    suite.sortResults[0] = bubbleSort(unsortedArr, datasetSize);
    suite.sortResults[1] = selectionSort(unsortedArr, datasetSize);
    suite.sortResults[2] = insertionSort(unsortedArr, datasetSize);
    suite.sortResults[3] = mergeSort(unsortedArr, datasetSize);
    suite.sortResults[4] = quickSort(unsortedArr, datasetSize);
    suite.sortResults[5] = heapSort(unsortedArr, datasetSize);
    suite.sortResults[6] = radixSort(unsortedArr, datasetSize);
    suite.sortResults[7] = shellSort(unsortedArr, datasetSize);
    suite.sortResults[8] = countingSort(unsortedArr, datasetSize);
    suite.sortResults[9] = bucketSort(unsortedArr, datasetSize);
    suite.sortResultCount = 10;

    free(arr);
    free(unsortedArr);
    return suite;
}

char* serializeBenchmarkJSON(BenchmarkSuite suite) {
    static char jsonBuf[16384];
    jsonBuf[0] = '\0';

    snprintf(jsonBuf, sizeof(jsonBuf),
             "{\"datasetSize\":%d,\"searches\":[", suite.datasetSize);

    for (int i = 0; i < suite.searchResultCount; i++) {
        char sBuf[512];
        snprintf(sBuf, sizeof(sBuf),
                 "{\"name\":\"%s\",\"comparisons\":%d,\"timeMs\":%.6f}",
                 suite.searchResults[i].algorithmName, suite.searchResults[i].comparisons, suite.searchResults[i].executionTimeMs);
        strcat(jsonBuf, sBuf);
        if (i < suite.searchResultCount - 1) strcat(jsonBuf, ",");
    }

    strcat(jsonBuf, "],\"sorts\":[");
    for (int i = 0; i < suite.sortResultCount; i++) {
        char sBuf[512];
        snprintf(sBuf, sizeof(sBuf),
                 "{\"name\":\"%s\",\"comparisons\":%ld,\"swaps\":%ld,\"timeMs\":%.6f}",
                 suite.sortResults[i].algorithmName, suite.sortResults[i].comparisons, suite.sortResults[i].swaps, suite.sortResults[i].executionTimeMs);
        strcat(jsonBuf, sBuf);
        if (i < suite.sortResultCount - 1) strcat(jsonBuf, ",");
        freeSortResult(&suite.sortResults[i]);
    }
    strcat(jsonBuf, "]}");

    return jsonBuf;
}
