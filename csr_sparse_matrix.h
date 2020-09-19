#ifndef CSR_SPARSE_MATRIX_H
#define CSR_SPARSE_MATRIX_H

/*
*  CSR_Sparse_Matrix Summary:
*  A struct that holds all relevant data for tracking Sparse matrix A, node degrees, F and all data for 
*  calculating B, B hat, shifts and the appropriate matrix multiplications.
*  Additionally contains all functions required for operating on such structures.
*
*  add_row - adds a new sparse row with given data to the sparse matrix.
*  add_ones_row - adds a new row of 1.0 values on the given columns to the sparse matrix.
*  get_c_max - calculates and populates the column max value needed for matrix shifting.
*  calc_f_vals - Calculate and store the F values for B hat
*  vector_vector_product - calculates the dot product of 2 vectors.
*  matrix_vector_product - calculates the matrix-vector product accounting for all B and B hat extra calculations.
*  vector_matrix_vector_product - calculates v1 * (B * v2) using the above 2 functions.
*  calculate_B_ij - returns the value of Bij.
*  get_subset - creates a new sparse matrix that represents a subset of the rows of columns of the original matrix.
*  create_sparse_matrix - creates a new, empty sparse matrix.
*  free_sparse_matrix - frees a sparse matrix and all of it's components.
*
*/

#include "error_handler.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#ifndef IS_POSITIVE
#define IS_POSITIVE(X) ((X) > 0.00001)
#endif

#ifndef MALLOC_ERROR
#define MALLOC_ERROR -1000000000000
#endif


typedef struct spmat
{
    double *data;
    double *f;
    int *row_inds;
    int *col_inds;
    int row_size;
    int col_size;
    int nnz_items;
    int *nodes_degrees;
    int *nodes_translation;
    int M;
    double c_max;
} sparse_matrix;

sparse_matrix *add_row(sparse_matrix *, int *, double *, int);

sparse_matrix *add_ones_row(sparse_matrix *, int *, int);

double get_c_max(sparse_matrix *); /* returns ||C||1 */

double *matrix_vector_product(sparse_matrix *, double *, int);

double calc_f_vals(sparse_matrix *);

double calculate_B_ij(sparse_matrix *, int, int);

sparse_matrix *create_sparse_matrix(int, int*);

double vector_vector_product(double *, double *, int);

double vector_matrix_vector_product(double *, sparse_matrix *, double *);

sparse_matrix *get_subset(sparse_matrix *, int *, int);

void free_sparse_matrix(sparse_matrix *sp);

#endif
