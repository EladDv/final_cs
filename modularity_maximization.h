/* THIS IS FOR ALGORITHM 2! */
#ifndef MODULARITY_MAX_H
#define MODULARITY_MAX_H

/*
*  Modularity_Maximization Summary:
*  A module handling dividing a single group to 2 modularity groups.  
*
*  divide_group_into_two - Divide a group into 2 groups via power iteration.
*  split_group - Apply power iteration + alg.4 to split a single groups into modularity 2 groups.
*  optimize_split - Optimize a group split via alg. 4.
*  update_score - updates the score efficiently
*
*/

#include "error_handler.h"
#include "csr_sparse_matrix.h"
#include "power_iter.h"
#include "queue.h"


double *divide_group_into_two(sparse_matrix *, int * is_eigen_val_positive);
queue *split_group(sparse_matrix *);
double *optimize_split(sparse_matrix *, double *);
void update_score(int, double *,sparse_matrix *, const double *);

#endif
