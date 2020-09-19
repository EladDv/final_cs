
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


#include "csr_sparse_matrix.h"

sparse_matrix *add_row(sparse_matrix *sp, int *col_inds, double *row_data, int row_nnz_items){
    int *new_row_inds, *new_col_inds;
    double *new_data;

    new_row_inds = (int *)malloc((sp->row_size + 2) * sizeof(int));
    if (new_row_inds == NULL){
        failed_alloc("add_row");
        free_sparse_matrix(sp);
        return NULL;
    }
    if (sp->row_inds != NULL){
        memcpy(new_row_inds, sp->row_inds, sp->row_size * sizeof(int));
    }
    new_row_inds[sp->row_size] = sp->nnz_items;
    new_row_inds[sp->row_size + 1] = sp->nnz_items + row_nnz_items;
    if (sp->row_inds != NULL)
    {
        free(sp->row_inds);
    }
    sp->row_inds = new_row_inds;


    if(row_nnz_items > 0)
    {
        new_col_inds = (int *)calloc((sp->nnz_items + row_nnz_items), sizeof(int));
        if (new_col_inds  == NULL){
            failed_alloc("add_row");
            free_sparse_matrix(sp);
            return NULL;
        }
        if (sp->row_inds != NULL){
            memcpy(new_col_inds, sp->col_inds, sp->nnz_items * sizeof(int));
        }
        memcpy(new_col_inds + sp->nnz_items, col_inds, row_nnz_items * sizeof(int)); /* copy new col inds to sp */
        if (sp->col_inds != NULL) free(sp->col_inds);
        sp->col_inds = new_col_inds;


        new_data = (double *)calloc((sp->nnz_items + row_nnz_items), sizeof(double));
        if (new_data == NULL){
            failed_alloc("add_row");
            free_sparse_matrix(sp);
            return NULL;
        }
        if (sp->data != NULL){
            memcpy(new_data, sp->data, sp->nnz_items * sizeof(double));
        }
        memcpy(new_data + sp->nnz_items, row_data, row_nnz_items * sizeof(double));  /* copy new row data to sp */
        if(sp->data != NULL) free(sp->data);
        sp->data = new_data;
    }

    sp->row_size = sp->row_size + 1;
    sp->nnz_items = sp->nnz_items + row_nnz_items;
    return sp;
}

sparse_matrix *add_ones_row(sparse_matrix *sp, int *col_inds, int row_nnz_items)
{
    int i;
    double *data = (double *)malloc(row_nnz_items * sizeof(double));
    if (data != NULL)
    {
        for (i = 0; i < row_nnz_items; i++)
        {
            data[i] = 1.0;
        }
        return add_row(sp, col_inds, data, row_nnz_items);
    }
    free_sparse_matrix(sp);
    return NULL;
}

double vector_matrix_vector_product(double *v1, sparse_matrix *sp, double *v2) /** don't need ? **/
{
    double *mv2;
    mv2 = matrix_vector_product(sp, v2,1); /* B^ */
    if (mv2 == NULL)
    {
        return MALLOC_ERROR;
    }
    return vector_vector_product(v1, mv2, sp->col_size);
}

double vector_vector_product(double *v1, double *v2, int length)
{
    int i;
    double result = 0;
    for (i = 0; i < length; i++)
    {
        result += v1[i] * v2[i];
    }
    return result;
}

double *matrix_vector_product(sparse_matrix *sp, double *v, int is_B_with_hat )
{ /* B * v */
    int low, high, i, j;
    double *result, result_sum_j, temp;
    
    result = (double *)calloc(sp->row_size, sizeof(double));
    if (!result)
    {
        failed_alloc("matrix_vector_product");
        return NULL;
    }
    result_sum_j = 0;
    for(i = 0; i < sp->row_size; i++)
    {
        result_sum_j -= v[i]*(double)sp->nodes_degrees[i] / ((double)sp->M);
    }

    for (i = 0; i < sp->row_size; i++)
    {
        low = sp->row_inds[i];
        high = sp->row_inds[i + 1];
        temp = (double)sp->nodes_degrees[i] * result_sum_j;
        if((is_B_with_hat==1)){
            temp += (sp->c_max - sp->f[i]) * v[i];
        }
        result[i] += temp;

        for (j = low; j < high; j++)
        {
            result[i] += sp->data[j] * v[sp->col_inds[j]]; /* B * v */
        }
    }
    return result;
}

double calc_f_vals(sparse_matrix *modularity_matrix)
/* sum over j of ki*kj / M  = ki * const, this calculates the */
{
    int i, j;
    double sum_kjm;

    sum_kjm = 0;
    for (j = 0; j < modularity_matrix->col_size; ++j)
    {
        sum_kjm += (double)(modularity_matrix->nodes_degrees[j]) / (double)modularity_matrix->M;
    }

    for (i = 0; i < modularity_matrix->col_size; ++i)
    {
        modularity_matrix->f[i] = (double)modularity_matrix->nodes_degrees[i] - (double)modularity_matrix->nodes_degrees[i] * sum_kjm;
    }

    return 1;
}

double calculate_B_ij(sparse_matrix *sub_matrix, int i, int j){
    double temp, A_ij;
    int low, high, k;

    temp = -1.0*((double)sub_matrix->nodes_degrees[i]*(double)sub_matrix->nodes_degrees[j]) / ((double)sub_matrix->M);
    low  = sub_matrix->row_inds[i];
    high = sub_matrix->row_inds[i+1];
    for(k=low; k<high; k++){
        if(j == sub_matrix->col_inds[k]){
            A_ij = sub_matrix->data[k];
        }
    }
    return A_ij +temp;
}


sparse_matrix *create_sparse_matrix(int col_size, int* subset)
{
    /*int *row_inds, *col_inds;
    double *data;*/
    int i;

    sparse_matrix *sp = (sparse_matrix *)malloc(sizeof(sparse_matrix));
    if (sp == NULL)
    {
        failed_alloc("create_sparse_matrix");
        return NULL;
    }

    sp->nodes_degrees = (int *)calloc(col_size, sizeof(int));
    if (sp->nodes_degrees == NULL)
    {
        failed_alloc("create_sparse_matrix");
        free(sp);
        return NULL;
    }

    sp->f = (double *)calloc(col_size, sizeof(double));
    if (sp->f == NULL)
    {
        failed_alloc("create_sparse_matrix");
        free(sp);
        free(sp->nodes_degrees);
        return NULL;
    }

    sp->nodes_translation = (int *)calloc(col_size, sizeof(int));
    if (sp->nodes_translation == NULL)
    {
        failed_alloc("create_sparse_matrix");
        free(sp);
        free(sp->nodes_degrees);
        free(sp->f);
        return NULL;
    }
    if(subset != NULL)
    {
        for (i = 0; i < col_size; i++)
        {
            sp->nodes_translation[i] = subset[i];
        }
        printf("subset, ");
        print_vector_int(sp->nodes_translation, col_size);
    }
    else
    {
        for (i = 0; i < col_size; i++)
        {
            sp->nodes_translation[i] = i;
        }
        printf("no subset, ");
        print_vector_int(sp->nodes_translation, col_size);
    }
    sp->row_inds = NULL;
    sp->col_inds = NULL;
    sp->data = NULL;
    sp->M = 0;
    sp->nnz_items = 0;
    sp->c_max = 0;
    sp->col_size = col_size;
    sp->row_size = 0;

    return sp;
}

double get_c_max(sparse_matrix *sp)
{ /* returns ||C||1 */

    int high, low, i, j;
    double *col_sums, c_max, temp;
    col_sums = (double *)calloc(sp->col_size, sizeof(double));
    if (col_sums == NULL)
    {
        failed_alloc("get_c_max");
        return -1.0;
    }

    printf("cmax 0\n");
    sp->c_max = 0.0;

    for (i = 0; i < sp->row_size; i++)
    {
        low = sp->row_inds[i];
        high = sp->row_inds[i+1];
        for (j = 0; j < sp->col_size; j++)
        {
            col_sums[j] = -1.0 * ((double)sp->nodes_degrees[i] * (double)sp->nodes_degrees[j]) / ((double)sp->M);
        }
        for(j = low; j < high; j++)
        {
            col_sums[sp->col_inds[j]] += 1;
        }
        temp = 0;
        for (j = 0; j < sp->col_size; j++)
        {
            temp += fabs(col_sums[j]);
        }
        /*printf("col_sum: %f, col_vector: ", temp);
        print_vector_int(sp->nodes_degrees, sp->col_size);
        print_vector_double(col_sums, sp->col_size);*/
        if(temp > c_max) c_max = temp;

    }
    printf("cmax 1\n");
    /*c_max = 0;
    for (i = 0; i < sp->col_size; i++)
    {
        if (c_max < fabs(col_sums[i]))
        {
            c_max = fabs(col_sums[i]);
        }
    }
    printf("cmax 2, %d\n", sp->col_size);
    print_vector_double(col_sums, sp->col_size);
    free(col_sums);
    printf("cmax 3\n");*/
    sp->c_max = c_max;
    return c_max;
}

sparse_matrix *get_subset(sparse_matrix *sp, int *subset, int subset_size)
{
    int i, j, k, m, high, low, num_nnz, *temp_col_inds;
    sparse_matrix *sub_sp;


    sub_sp = create_sparse_matrix(subset_size, subset);
    if (sub_sp == NULL)
    {
        return NULL;
    }

    for (i = 0; i < subset_size; i++)
    {
        low = sp->row_inds[subset[i]];
        high = sp->row_inds[subset[i] + 1];
        num_nnz = 0;
        for (j = low; j < high; j++)
        {
            for (k = 0; k < subset_size; k++)
            {
                if (subset[k] == sp->col_inds[j])
                {
                    ++num_nnz;
                    break;
                }
            }
        }

        temp_col_inds = (int *)calloc(num_nnz, sizeof(int));
        if (temp_col_inds == NULL)
        {
            failed_alloc("get_subset");
            free_sparse_matrix(sub_sp);
            return NULL;
        }
        m = 0;
        for (j = low; j < high; j++)
        {
            for (k = 0; k < subset_size; k++)
            {
                if (subset[k] == sp->col_inds[j])
                {
                    temp_col_inds[m] = k;
                    m++;
                }
            }
        }
        if (add_ones_row(sub_sp, temp_col_inds, num_nnz) == NULL)
        {
            free(temp_col_inds);
            return NULL;
        }
        sub_sp->nodes_degrees[i] = sp->nodes_degrees[subset[i]];
    }
    sub_sp->M = sp->M;
    sub_sp->c_max = sp->c_max;
    free(temp_col_inds);
    return sub_sp;
}


void free_sparse_matrix(sparse_matrix *sp)
{
    free(sp->data);
    free(sp->col_inds);
    free(sp->row_inds);
    free(sp->nodes_translation);
    free(sp->nodes_degrees);
    free(sp->f);
    free(sp);
}
