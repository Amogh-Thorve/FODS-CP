#include "sparse_matrix.h"

SparseMatrix createSparseMatrix(int rows, int cols) {
    SparseMatrix sm;
    sm.rows = rows;
    sm.cols = cols;
    sm.numElements = 0;
    return sm;
}

void insertSparseElement(SparseMatrix *sm, int row, int col, double val) {
    if (!sm || sm->numElements >= 200 || val == 0.0) return;
    sm->elements[sm->numElements].row = row;
    sm->elements[sm->numElements].col = col;
    sm->elements[sm->numElements].val = val;
    sm->numElements++;
}

void displaySparseMatrix(SparseMatrix *sm) {
    if (!sm) return;
    printf("SparseMatrix (%dx%d, Non-zero: %d):\n", sm->rows, sm->cols, sm->numElements);
    for (int i = 0; i < sm->numElements; i++) {
        printf("Row: %d, Col: %d -> Val: %.2f\n", sm->elements[i].row, sm->elements[i].col, sm->elements[i].val);
    }
}

char* serializeSparseMatrixJSON(SparseMatrix *sm) {
    static char jsonBuf[4096];
    jsonBuf[0] = '\0';
    snprintf(jsonBuf, sizeof(jsonBuf),
             "{\"rows\":%d,\"cols\":%d,\"numElements\":%d,\"triplets\":[",
             sm->rows, sm->cols, sm->numElements);

    for (int i = 0; i < sm->numElements; i++) {
        char tripBuf[128];
        snprintf(tripBuf, sizeof(tripBuf),
                 "{\"row\":%d,\"col\":%d,\"val\":%.2f}",
                 sm->elements[i].row, sm->elements[i].col, sm->elements[i].val);
        strcat(jsonBuf, tripBuf);
        if (i < sm->numElements - 1) strcat(jsonBuf, ",");
    }
    strcat(jsonBuf, "]}");
    return jsonBuf;
}
