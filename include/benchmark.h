#ifndef BENCHMARK_H
#define BENCHMARK_H

#include "search.h"
#include "sorting.h"

typedef struct {
    int datasetSize;
    SearchResult searchResults[5]; // Linear, Binary, Indexed, Interpolation, Fibonacci
    int searchResultCount;
    SortResult sortResults[10];   // 10 sorting algorithms
    int sortResultCount;
} BenchmarkSuite;

BenchmarkSuite runFullBenchmark(int datasetSize);
char* serializeBenchmarkJSON(BenchmarkSuite suite);

#endif // BENCHMARK_H
