#ifndef SPARSE_MATRIX_H
#define SPARSE_MATRIX_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int row;
    int col;
    double val;
} MatrixElement;

typedef struct {
    int rows;
    int cols;
    int numElements;
    MatrixElement elements[200];
} SparseMatrix;

SparseMatrix createSparseMatrix(int rows, int cols);
void insertSparseElement(SparseMatrix *sm, int row, int col, double val);
void displaySparseMatrix(SparseMatrix *sm);
char* serializeSparseMatrixJSON(SparseMatrix *sm);

#endif // SPARSE_MATRIX_H
