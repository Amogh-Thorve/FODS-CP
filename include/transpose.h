#ifndef TRANSPOSE_H
#define TRANSPOSE_H

#include "sparse_matrix.h"

typedef struct {
    SparseMatrix transposedMatrix;
    double executionTimeMs;
    long operationsCount;
} TransposeResult;

TransposeResult simpleTranspose(SparseMatrix *sm);
TransposeResult fastTranspose(SparseMatrix *sm);
char* serializeTransposeBenchmarkJSON(TransposeResult simpleRes, TransposeResult fastRes);

#endif // TRANSPOSE_H
