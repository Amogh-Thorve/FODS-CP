#include "transpose.h"

#ifdef _WIN32
#include <windows.h>
static double getHighResTimeMsTrans(LARGE_INTEGER start, LARGE_INTEGER end, LARGE_INTEGER freq) {
    return (double)(end.QuadPart - start.QuadPart) * 1000.0 / (double)freq.QuadPart;
}
#endif

// Simple Transpose - O(cols * numElements)
TransposeResult simpleTranspose(SparseMatrix *sm) {
    TransposeResult res;
    memset(&res, 0, sizeof(res));
    res.transposedMatrix.rows = sm->cols;
    res.transposedMatrix.cols = sm->rows;
    res.transposedMatrix.numElements = sm->numElements;

#ifdef _WIN32
    LARGE_INTEGER freq, start, end;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&start);
#endif

    int k = 0;
    for (int col = 0; col < sm->cols; col++) {
        for (int i = 0; i < sm->numElements; i++) {
            res.operationsCount++;
            if (sm->elements[i].col == col) {
                res.transposedMatrix.elements[k].row = sm->elements[i].col;
                res.transposedMatrix.elements[k].col = sm->elements[i].row;
                res.transposedMatrix.elements[k].val = sm->elements[i].val;
                k++;
            }
        }
    }

#ifdef _WIN32
    QueryPerformanceCounter(&end);
    res.executionTimeMs = getHighResTimeMsTrans(start, end, freq);
#endif

    return res;
}

// Fast Transpose - O(cols + numElements)
TransposeResult fastTranspose(SparseMatrix *sm) {
    TransposeResult res;
    memset(&res, 0, sizeof(res));
    res.transposedMatrix.rows = sm->cols;
    res.transposedMatrix.cols = sm->rows;
    res.transposedMatrix.numElements = sm->numElements;

#ifdef _WIN32
    LARGE_INTEGER freq, start, end;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&start);
#endif

    int rowTerms[200] = {0};
    int startingPos[200] = {0};

    for (int i = 0; i < sm->numElements; i++) {
        rowTerms[sm->elements[i].col]++;
        res.operationsCount++;
    }

    startingPos[0] = 0;
    for (int i = 1; i < sm->cols; i++) {
        startingPos[i] = startingPos[i - 1] + rowTerms[i - 1];
        res.operationsCount++;
    }

    for (int i = 0; i < sm->numElements; i++) {
        int j = startingPos[sm->elements[i].col]++;
        res.transposedMatrix.elements[j].row = sm->elements[i].col;
        res.transposedMatrix.elements[j].col = sm->elements[i].row;
        res.transposedMatrix.elements[j].val = sm->elements[i].val;
        res.operationsCount++;
    }

#ifdef _WIN32
    QueryPerformanceCounter(&end);
    res.executionTimeMs = getHighResTimeMsTrans(start, end, freq);
#endif

    return res;
}

char* serializeTransposeBenchmarkJSON(TransposeResult simpleRes, TransposeResult fastRes) {
    static char jsonBuf[2048];
    snprintf(jsonBuf, sizeof(jsonBuf),
             "{\"simpleTranspose\":{\"executionTimeMs\":%.6f,\"operationsCount\":%ld},\"fastTranspose\":{\"executionTimeMs\":%.6f,\"operationsCount\":%ld}}",
             simpleRes.executionTimeMs, simpleRes.operationsCount, fastRes.executionTimeMs, fastRes.operationsCount);
    return jsonBuf;
}
