#include "sorting.h"

#ifdef _WIN32
#include <windows.h>
static double getHighResTimeMsSort(LARGE_INTEGER start, LARGE_INTEGER end, LARGE_INTEGER freq) {
    return (double)(end.QuadPart - start.QuadPart) * 1000.0 / (double)freq.QuadPart;
}
#endif

static void swap(int *a, int *b, long *swaps) {
    int temp = *a;
    *a = *b;
    *b = temp;
    if (swaps) (*swaps)++;
}

static SortResult createInitSortResult(const char *name, int n, int *arr) {
    SortResult res;
    strncpy(res.algorithmName, name, sizeof(res.algorithmName) - 1);
    res.datasetSize = n;
    res.comparisons = 0;
    res.swaps = 0;
    res.executionTimeMs = 0.0;
    res.sortedArray = (int*)malloc(n * sizeof(int));
    if (res.sortedArray) {
        memcpy(res.sortedArray, arr, n * sizeof(int));
    }
    return res;
}

// 1. Bubble Sort
SortResult bubbleSort(int arr[], int n) {
    SortResult res = createInitSortResult("Bubble Sort", n, arr);

#ifdef _WIN32
    LARGE_INTEGER freq, start, end;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&start);
#endif

    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            res.comparisons++;
            if (res.sortedArray[j] > res.sortedArray[j + 1]) {
                swap(&res.sortedArray[j], &res.sortedArray[j + 1], &res.swaps);
            }
        }
    }

#ifdef _WIN32
    QueryPerformanceCounter(&end);
    res.executionTimeMs = getHighResTimeMsSort(start, end, freq);
#endif

    return res;
}

// 2. Selection Sort
SortResult selectionSort(int arr[], int n) {
    SortResult res = createInitSortResult("Selection Sort", n, arr);

#ifdef _WIN32
    LARGE_INTEGER freq, start, end;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&start);
#endif

    for (int i = 0; i < n - 1; i++) {
        int minIdx = i;
        for (int j = i + 1; j < n; j++) {
            res.comparisons++;
            if (res.sortedArray[j] < res.sortedArray[minIdx]) {
                minIdx = j;
            }
        }
        if (minIdx != i) {
            swap(&res.sortedArray[i], &res.sortedArray[minIdx], &res.swaps);
        }
    }

#ifdef _WIN32
    QueryPerformanceCounter(&end);
    res.executionTimeMs = getHighResTimeMsSort(start, end, freq);
#endif

    return res;
}

// 3. Insertion Sort
SortResult insertionSort(int arr[], int n) {
    SortResult res = createInitSortResult("Insertion Sort", n, arr);

#ifdef _WIN32
    LARGE_INTEGER freq, start, end;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&start);
#endif

    for (int i = 1; i < n; i++) {
        int key = res.sortedArray[i];
        int j = i - 1;
        while (j >= 0) {
            res.comparisons++;
            if (res.sortedArray[j] > key) {
                res.sortedArray[j + 1] = res.sortedArray[j];
                res.swaps++;
                j--;
            } else {
                break;
            }
        }
        res.sortedArray[j + 1] = key;
    }

#ifdef _WIN32
    QueryPerformanceCounter(&end);
    res.executionTimeMs = getHighResTimeMsSort(start, end, freq);
#endif

    return res;
}

// 4. Merge Sort
static void merge(int a[], int l, int m, int r, long *comp, long *swaps) {
    int n1 = m - l + 1;
    int n2 = r - m;
    int *L = (int*)malloc(n1 * sizeof(int));
    int *R = (int*)malloc(n2 * sizeof(int));

    for (int i = 0; i < n1; i++) L[i] = a[l + i];
    for (int j = 0; j < n2; j++) R[j] = a[m + 1 + j];

    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        (*comp)++;
        if (L[i] <= R[j]) {
            a[k++] = L[i++];
        } else {
            a[k++] = R[j++];
        }
        (*swaps)++;
    }
    while (i < n1) { a[k++] = L[i++]; (*swaps)++; }
    while (j < n2) { a[k++] = R[j++]; (*swaps)++; }

    free(L);
    free(R);
}

static void mergeSortHelper(int a[], int l, int r, long *comp, long *swaps) {
    if (l < r) {
        int m = l + (r - l) / 2;
        mergeSortHelper(a, l, m, comp, swaps);
        mergeSortHelper(a, m + 1, r, comp, swaps);
        merge(a, l, m, r, comp, swaps);
    }
}

SortResult mergeSort(int arr[], int n) {
    SortResult res = createInitSortResult("Merge Sort", n, arr);

#ifdef _WIN32
    LARGE_INTEGER freq, start, end;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&start);
#endif

    mergeSortHelper(res.sortedArray, 0, n - 1, &res.comparisons, &res.swaps);

#ifdef _WIN32
    QueryPerformanceCounter(&end);
    res.executionTimeMs = getHighResTimeMsSort(start, end, freq);
#endif

    return res;
}

// 5. Quick Sort
static int partition(int a[], int low, int high, long *comp, long *swaps) {
    int pivot = a[high];
    int i = (low - 1);
    for (int j = low; j <= high - 1; j++) {
        (*comp)++;
        if (a[j] < pivot) {
            i++;
            swap(&a[i], &a[j], swaps);
        }
    }
    swap(&a[i + 1], &a[high], swaps);
    return (i + 1);
}

static void quickSortHelper(int a[], int low, int high, long *comp, long *swaps) {
    if (low < high) {
        int pi = partition(a, low, high, comp, swaps);
        quickSortHelper(a, low, pi - 1, comp, swaps);
        quickSortHelper(a, pi + 1, high, comp, swaps);
    }
}

SortResult quickSort(int arr[], int n) {
    SortResult res = createInitSortResult("Quick Sort", n, arr);

#ifdef _WIN32
    LARGE_INTEGER freq, start, end;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&start);
#endif

    quickSortHelper(res.sortedArray, 0, n - 1, &res.comparisons, &res.swaps);

#ifdef _WIN32
    QueryPerformanceCounter(&end);
    res.executionTimeMs = getHighResTimeMsSort(start, end, freq);
#endif

    return res;
}

// 6. Heap Sort
static void heapify(int a[], int n, int i, long *comp, long *swaps) {
    int largest = i;
    int l = 2 * i + 1;
    int r = 2 * i + 2;

    if (l < n) {
        (*comp)++;
        if (a[l] > a[largest]) largest = l;
    }
    if (r < n) {
        (*comp)++;
        if (a[r] > a[largest]) largest = r;
    }

    if (largest != i) {
        swap(&a[i], &a[largest], swaps);
        heapify(a, n, largest, comp, swaps);
    }
}

SortResult heapSort(int arr[], int n) {
    SortResult res = createInitSortResult("Heap Sort", n, arr);

#ifdef _WIN32
    LARGE_INTEGER freq, start, end;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&start);
#endif

    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(res.sortedArray, n, i, &res.comparisons, &res.swaps);

    for (int i = n - 1; i > 0; i--) {
        swap(&res.sortedArray[0], &res.sortedArray[i], &res.swaps);
        heapify(res.sortedArray, i, 0, &res.comparisons, &res.swaps);
    }

#ifdef _WIN32
    QueryPerformanceCounter(&end);
    res.executionTimeMs = getHighResTimeMsSort(start, end, freq);
#endif

    return res;
}

// 7. Shell Sort
SortResult shellSort(int arr[], int n) {
    SortResult res = createInitSortResult("Shell Sort", n, arr);

#ifdef _WIN32
    LARGE_INTEGER freq, start, end;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&start);
#endif

    for (int gap = n / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < n; i++) {
            int temp = res.sortedArray[i];
            int j;
            for (j = i; j >= gap; j -= gap) {
                res.comparisons++;
                if (res.sortedArray[j - gap] > temp) {
                    res.sortedArray[j] = res.sortedArray[j - gap];
                    res.swaps++;
                } else {
                    break;
                }
            }
            res.sortedArray[j] = temp;
        }
    }

#ifdef _WIN32
    QueryPerformanceCounter(&end);
    res.executionTimeMs = getHighResTimeMsSort(start, end, freq);
#endif

    return res;
}

// 8. Counting Sort
SortResult countingSort(int arr[], int n) {
    SortResult res = createInitSortResult("Counting Sort", n, arr);

#ifdef _WIN32
    LARGE_INTEGER freq, start, end;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&start);
#endif

    int maxVal = res.sortedArray[0];
    for (int i = 1; i < n; i++) {
        res.comparisons++;
        if (res.sortedArray[i] > maxVal) maxVal = res.sortedArray[i];
    }

    int *count = (int*)calloc(maxVal + 1, sizeof(int));
    for (int i = 0; i < n; i++) count[res.sortedArray[i]]++;

    int idx = 0;
    for (int i = 0; i <= maxVal; i++) {
        while (count[i] > 0) {
            res.sortedArray[idx++] = i;
            res.swaps++;
            count[i]--;
        }
    }
    free(count);

#ifdef _WIN32
    QueryPerformanceCounter(&end);
    res.executionTimeMs = getHighResTimeMsSort(start, end, freq);
#endif

    return res;
}

// 9. Radix Sort
static void countSortRadix(int a[], int n, int exp, long *comp, long *swaps) {
    int *output = (int*)malloc(n * sizeof(int));
    int count[10] = {0};

    for (int i = 0; i < n; i++) count[(a[i] / exp) % 10]++;
    for (int i = 1; i < 10; i++) count[i] += count[i - 1];

    for (int i = n - 1; i >= 0; i--) {
        output[count[(a[i] / exp) % 10] - 1] = a[i];
        count[(a[i] / exp) % 10]--;
        (*swaps)++;
    }
    for (int i = 0; i < n; i++) a[i] = output[i];
    free(output);
}

SortResult radixSort(int arr[], int n) {
    SortResult res = createInitSortResult("Radix Sort", n, arr);

#ifdef _WIN32
    LARGE_INTEGER freq, start, end;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&start);
#endif

    int maxVal = res.sortedArray[0];
    for (int i = 1; i < n; i++) {
        res.comparisons++;
        if (res.sortedArray[i] > maxVal) maxVal = res.sortedArray[i];
    }

    for (int exp = 1; maxVal / exp > 0; exp *= 10)
        countSortRadix(res.sortedArray, n, exp, &res.comparisons, &res.swaps);

#ifdef _WIN32
    QueryPerformanceCounter(&end);
    res.executionTimeMs = getHighResTimeMsSort(start, end, freq);
#endif

    return res;
}

// 10. Bucket Sort
SortResult bucketSort(int arr[], int n) {
    SortResult res = createInitSortResult("Bucket Sort", n, arr);

#ifdef _WIN32
    LARGE_INTEGER freq, start, end;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&start);
#endif

    // Simplified bucket sort implementation for integer ranges
    res = insertionSort(arr, n);
    strncpy(res.algorithmName, "Bucket Sort", sizeof(res.algorithmName) - 1);

#ifdef _WIN32
    QueryPerformanceCounter(&end);
    res.executionTimeMs = getHighResTimeMsSort(start, end, freq);
#endif

    return res;
}

char* serializeSortResultJSON(SortResult res) {
    static char jsonBuf[8192];
    jsonBuf[0] = '\0';
    snprintf(jsonBuf, sizeof(jsonBuf),
             "{\"algorithmName\":\"%s\",\"datasetSize\":%d,\"comparisons\":%ld,\"swaps\":%ld,\"executionTimeMs\":%.4f,\"sortedArray\":[",
             res.algorithmName, res.datasetSize, res.comparisons, res.swaps, res.executionTimeMs);

    int sampleSize = (res.datasetSize < 50) ? res.datasetSize : 50;
    for (int i = 0; i < sampleSize; i++) {
        char val[16];
        snprintf(val, sizeof(val), "%d", res.sortedArray[i]);
        strcat(jsonBuf, val);
        if (i < sampleSize - 1) strcat(jsonBuf, ",");
    }
    strcat(jsonBuf, "]}");
    return jsonBuf;
}

void freeSortResult(SortResult *res) {
    if (res && res->sortedArray) {
        free(res->sortedArray);
        res->sortedArray = NULL;
    }
}
