#include "search.h"
#include <math.h>

#ifdef _WIN32
#include <windows.h>
static double getHighResTimeMs(LARGE_INTEGER start, LARGE_INTEGER end, LARGE_INTEGER freq) {
    return (double)(end.QuadPart - start.QuadPart) * 1000.0 / (double)freq.QuadPart;
}
#endif

SearchResult linearSearch(int arr[], int n, int key) {
    SearchResult res;
    strcpy(res.algorithmName, "Linear Search");
    res.targetId = key;
    res.foundIndex = -1;
    res.comparisons = 0;
    res.executionTimeMs = 0.0;

#ifdef _WIN32
    LARGE_INTEGER freq, start, end;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&start);
#else
    clock_t start = clock();
#endif

    for (int i = 0; i < n; i++) {
        res.comparisons++;
        if (arr[i] == key) {
            res.foundIndex = i;
            break;
        }
    }

#ifdef _WIN32
    QueryPerformanceCounter(&end);
    res.executionTimeMs = getHighResTimeMs(start, end, freq);
#else
    clock_t end = clock();
    res.executionTimeMs = (double)(end - start) * 1000.0 / CLOCKSPERSEC;
#endif

    return res;
}

SearchResult binarySearch(int arr[], int n, int key) {
    SearchResult res;
    strcpy(res.algorithmName, "Binary Search");
    res.targetId = key;
    res.foundIndex = -1;
    res.comparisons = 0;

#ifdef _WIN32
    LARGE_INTEGER freq, start, end;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&start);
#else
    clock_t start = clock();
#endif

    int low = 0, high = n - 1;
    while (low <= high) {
        res.comparisons++;
        int mid = low + (high - low) / 2;
        if (arr[mid] == key) {
            res.foundIndex = mid;
            break;
        } else if (arr[mid] < key) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

#ifdef _WIN32
    QueryPerformanceCounter(&end);
    res.executionTimeMs = getHighResTimeMs(start, end, freq);
#else
    clock_t end = clock();
    res.executionTimeMs = (double)(end - start) * 1000.0 / CLOCKSPERSEC;
#endif

    return res;
}

SearchResult indexedSequentialSearch(int arr[], int n, int key) {
    SearchResult res;
    strcpy(res.algorithmName, "Indexed Sequential Search");
    res.targetId = key;
    res.foundIndex = -1;
    res.comparisons = 0;

#ifdef _WIN32
    LARGE_INTEGER freq, start, end;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&start);
#else
    clock_t start = clock();
#endif

    int blockSize = (int)sqrt(n);
    if (blockSize < 1) blockSize = 1;

    int index[50];
    int indexPos[50];
    int numBlocks = 0;

    for (int i = 0; i < n; i += blockSize) {
        index[numBlocks] = arr[i];
        indexPos[numBlocks] = i;
        numBlocks++;
    }

    int blockStart = 0;
    int blockEnd = n;
    for (int i = 0; i < numBlocks; i++) {
        res.comparisons++;
        if (key < index[i]) {
            blockEnd = indexPos[i];
            break;
        }
        blockStart = indexPos[i];
    }

    for (int i = blockStart; i < blockEnd && i < n; i++) {
        res.comparisons++;
        if (arr[i] == key) {
            res.foundIndex = i;
            break;
        }
    }

#ifdef _WIN32
    QueryPerformanceCounter(&end);
    res.executionTimeMs = getHighResTimeMs(start, end, freq);
#else
    clock_t end = clock();
    res.executionTimeMs = (double)(end - start) * 1000.0 / CLOCKSPERSEC;
#endif

    return res;
}

SearchResult interpolationSearch(int arr[], int n, int key) {
    SearchResult res;
    strcpy(res.algorithmName, "Interpolation Search");
    res.targetId = key;
    res.foundIndex = -1;
    res.comparisons = 0;

#ifdef _WIN32
    LARGE_INTEGER freq, start, end;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&start);
#else
    clock_t start = clock();
#endif

    int low = 0, high = n - 1;
    while (low <= high && key >= arr[low] && key <= arr[high]) {
        res.comparisons++;
        if (low == high) {
            if (arr[low] == key) res.foundIndex = low;
            break;
        }
        int pos = low + (((double)(high - low) / (arr[high] - arr[low])) * (key - arr[low]));
        if (pos < low || pos > high) break;

        if (arr[pos] == key) {
            res.foundIndex = pos;
            break;
        }
        if (arr[pos] < key) low = pos + 1;
        else high = pos - 1;
    }

#ifdef _WIN32
    QueryPerformanceCounter(&end);
    res.executionTimeMs = getHighResTimeMs(start, end, freq);
#else
    clock_t end = clock();
    res.executionTimeMs = (double)(end - start) * 1000.0 / CLOCKSPERSEC;
#endif

    return res;
}

SearchResult fibonacciSearch(int arr[], int n, int key) {
    SearchResult res;
    strcpy(res.algorithmName, "Fibonacci Search");
    res.targetId = key;
    res.foundIndex = -1;
    res.comparisons = 0;

#ifdef _WIN32
    LARGE_INTEGER freq, start, end;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&start);
#else
    clock_t start = clock();
#endif

    int fibM2 = 0; // (m-2)'th Fibonacci No.
    int fibM1 = 1; // (m-1)'th Fibonacci No.
    int fibM = fibM2 + fibM1; // m'th Fibonacci

    while (fibM < n) {
        fibM2 = fibM1;
        fibM1 = fibM;
        fibM = fibM2 + fibM1;
    }

    int offset = -1;
    while (fibM > 1) {
        res.comparisons++;
        int i = (offset + fibM2 < n - 1) ? (offset + fibM2) : (n - 1);

        if (arr[i] < key) {
            fibM = fibM1;
            fibM1 = fibM2;
            fibM2 = fibM - fibM1;
            offset = i;
        } else if (arr[i] > key) {
            fibM = fibM2;
            fibM1 = fibM1 - fibM2;
            fibM2 = fibM - fibM1;
        } else {
            res.foundIndex = i;
            break;
        }
    }

    if (fibM1 && offset + 1 < n && arr[offset + 1] == key) {
        res.foundIndex = offset + 1;
    }

#ifdef _WIN32
    QueryPerformanceCounter(&end);
    res.executionTimeMs = getHighResTimeMs(start, end, freq);
#else
    clock_t end = clock();
    res.executionTimeMs = (double)(end - start) * 1000.0 / CLOCKSPERSEC;
#endif

    return res;
}

char* serializeSearchResultJSON(SearchResult res) {
    static char jsonBuf[1024];
    snprintf(jsonBuf, sizeof(jsonBuf),
             "{\"algorithmName\":\"%s\",\"targetId\":%d,\"foundIndex\":%d,\"comparisons\":%d,\"executionTimeMs\":%.4f}",
             res.algorithmName, res.targetId, res.foundIndex, res.comparisons, res.executionTimeMs);
    return jsonBuf;
}
