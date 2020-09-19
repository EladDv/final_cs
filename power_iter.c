#include "power_iter.h"
/*
*  Power_iter Summary:
*  A module that implements the power iteration algorithm. 
*
*  power_iteration_step - A single step of the power iteration alg.
*  power_iteration - calculates and returns the leading eigen vector and eigen value of the given matrix.
*  frand - retuns a random double between 0.0 and 1.0.
*
*/


double *power_iteration_step(sparse_matrix *sp_mat, double *v, int v_size)
{
    double *A_dot_v;
    int i;
    double AdV_size;
    A_dot_v = matrix_vector_product(sp_mat, v, 1); /* B^ */

    if(A_dot_v == NULL)
    {
        failed_alloc("power_iteration_step");
        return NULL;
    }

    AdV_size = sqrt(vector_vector_product(A_dot_v, A_dot_v, v_size));
    for (i = 0; i < v_size; i++)
    {
        A_dot_v[i] = A_dot_v[i] / AdV_size; /* normalize */
    }
    return A_dot_v;
}

double *power_iteration(sparse_matrix *sp_mat, double *eigen_value_out)
{
    double *eigen_vector, *prev_eigen;
    int i, inf_loop_guard;
    double diff, eigen_value;

    calc_f_vals(sp_mat);
    printf("f: ");
    print_vector_double(sp_mat->f, sp_mat->col_size);
    /*for (i = 0; i < sp_mat->col_size; ++i)
    {  fi is the sum of elements in row i, sum j A[i][j] = ki, sum j ki*kj/M = ki * sum j(kj/M)
        sp_mat->f[i] =  (double)sp_mat->nodes_degrees[i] + (double)sp_mat->nodes_degrees[i] * sum_kjm;
    }*/
    eigen_vector = calloc(sp_mat->col_size, sizeof(double));
    if (eigen_vector == NULL)
    {
        return NULL;
    }

    prev_eigen = calloc(sp_mat->col_size, sizeof(double));
    if (prev_eigen == NULL)
    {
        free(eigen_vector);
        return NULL;
    }

    for (i = 0; i < sp_mat->col_size; i++)
    {
        /*eigen_vector[i] = i;*/
        eigen_vector[i] = frand();/* */
    }

    printf("starting loop\n");
    inf_loop_guard = -1;
    do
    {
        ++inf_loop_guard;
        prev_eigen = eigen_vector;
        eigen_vector = power_iteration_step(sp_mat, prev_eigen, sp_mat->col_size);
        diff = 0;
        for (i = 0; i < sp_mat->col_size; i++)
        {
            diff += fabs(fabs(eigen_vector[i]) - fabs(prev_eigen[i]));
        }
        diff = fabs(diff);
    } while (IS_POSITIVE(diff) && inf_loop_guard <= (50000 + sp_mat->col_size * sp_mat->col_size));

    printf("out of loop\n");

    eigen_value = vector_matrix_vector_product(eigen_vector, sp_mat, eigen_vector);
    print_vector_double(eigen_vector, sp_mat->col_size);/* */
    /*printf("eigen_val: %f\n", eigen_value);
    printf("eigen norm: %f\n", vector_vector_product(eigen_vector, eigen_vector, sp_mat->col_size));
    eigen_value = eigen_value / vector_vector_product(eigen_vector, eigen_vector, sp_mat->col_size);
    printf("eigen_val: %f\n", eigen_value); */
    eigen_value = eigen_value - sp_mat->c_max;
    printf("eigen_val: %f\n", eigen_value);
    if (eigen_value_out != NULL)
    {
        *eigen_value_out = eigen_value;
    }
    free(prev_eigen);
/*    print_vector_double(eigen_vector, sp_mat->col_size);
    printf("eigen_val: %f\n", eigen_value);*/
    return eigen_vector;
}

double frand()
{
    return (double)rand() / RAND_MAX;
    /*return ((double)rand() / RAND_MAX - 0.5) * 2.0;*/
}
