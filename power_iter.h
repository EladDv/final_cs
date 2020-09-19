#ifndef POWER_ITER
#define POWER_ITER
/*
*  Power_iter Summary:
*  A module that implements the power iteration algorithm. 
*
*  power_iteration_step - A single step of the power iteration alg.
*  power_iteration - calculates and returns the leading eigen vector and eigen value of the given matrix.
*  frand - retuns a random double between 0.0 and 1.0.
*
*/

#include "csr_sparse_matrix.h"
#include "error_handler.h"
#include "modularity_maximization.h"
#include <math.h>

#ifndef IS_INF_LOOP
#define IS_INF_LOOP 100000
#endif

double* power_iteration_step(sparse_matrix*, double*, int);
double* power_iteration(sparse_matrix*, double*);
double dot(double*, double*, int);
double frand();
 
#endif
