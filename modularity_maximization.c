/* THIS IS FOR ALGORITHM 2! */
#include "modularity_maximization.h"

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

double *divide_group_into_two(sparse_matrix *modularity_matrix, int *is_eigen_val_positive)
{ /* B^ is the sub mod_matrix of B */
    double *eigen_vector;
    double *eigen_value;
    double eigen_val;
    double *s_division_vector;
    int i;
    double sBs;
    int vector_len = modularity_matrix->col_size;

    if (is_eigen_val_positive != NULL){
        *is_eigen_val_positive = 1;
    }

    eigen_value = &eigen_val;
    /* Compute leading eigen-pair of the modularity matrix Bb[g]*/
    printf("divide_group_into_two -1\n");
    eigen_vector = power_iteration(modularity_matrix, eigen_value); /* power iter handles the internals of B^g */
    printf("divide_group_into_two 0\n");

    s_division_vector = (double *)malloc(vector_len * sizeof(double));

    if (!IS_POSITIVE(*eigen_value))
    { /*The group g is indivisible, s = {1, . . . ,1}*/
        for (i = 0; i < vector_len; ++i)
        {
            s_division_vector[i] = 1;
        }
        *is_eigen_val_positive =0;
        return s_division_vector;
    }
    /* Compute s = {s1, . . . , sn} where si ∈ {+1, −1}, according to eigen_vector*/
    for (i = 0; i < vector_len; ++i)
    {
        if (IS_POSITIVE(eigen_vector[i]))
        {
            s_division_vector[i] = 1;
        }
        else
        {
            s_division_vector[i] = -1;
        }
    }
    printf("divide_group_into_two 1\n");
    sBs = vector_matrix_vector_product(s_division_vector, modularity_matrix, s_division_vector);
    if (sBs == MALLOC_ERROR)
    {
        free(s_division_vector);
        return NULL;
    }
    printf("divide_group_into_two 2\n");
    if (!IS_POSITIVE(sBs))
    { /* The group g is indivisible*/
        for (i = 0; i < vector_len; ++i)
        {
            s_division_vector[i] = 1;
        }
        return s_division_vector;
    }
    /* return a division of g into two groups according to s*/
    return s_division_vector;
}

queue *split_group(sparse_matrix *sp_mat) /* turn into form that's easier to output */
{
    double *s;
    int *is_eigen_val_positive;
    int is_posi;
    queue *out_q;
    int i, t, count_g1, count_g2, *g1, *g2;
    out_q = init_q();
    if (out_q == NULL)
        return NULL;

    printf("split_group 0\n");

    is_eigen_val_positive = &is_posi;

    s = divide_group_into_two(sp_mat, is_eigen_val_positive);
    printf("split_group 1\n");

    count_g1 = 0;
    for (i = 0; i < sp_mat->col_size; i++){
        if (s[i] == 1.0)
            ++count_g1;
    }

    printf("------before optimize_split----\n");
    printf("g1 length: %d, g2 length: %d\n", count_g1, sp_mat->col_size - count_g1);
    /*print_vector_double(s,sp_mat->row_size);*/
    if (*is_eigen_val_positive == 1) {
        s = optimize_split(sp_mat, s);
    }
    printf("------after optimize_split----\n");
    /*print_vector_double(s,sp_mat->row_size);*/

    count_g1 = 0;
    for (i = 0; i < sp_mat->col_size; i++)
    {
        if (s[i] == 1)
            ++count_g1;
    }

    g1 = (int *)calloc(count_g1, sizeof(int));
    if (g1 == NULL)
    {
        free_q(out_q);
        return NULL;
    }
    g2 = (int *)calloc(sp_mat->col_size - count_g1, sizeof(int));
    if (g2 == NULL)
    {
        free(g1);
        free_q(out_q);
        return NULL;
    }

    t = count_g1;
    count_g1 = 0;
    count_g2 = 0;

    for (i = 0; i < sp_mat->col_size; ++i)
    {
        if (s[i] == 1)
        {
            g1[count_g1] = i;
            ++count_g1;
        }
        else
        {
            g2[count_g2] = i;
            ++count_g2;
        }
    }
    enqueue(out_q, g1, t);
    enqueue(out_q, g2, sp_mat->col_size - t);
    printf("g1 length: %d, g2 length: %d\n", t, sp_mat->col_size - t);
    return out_q;
}

void update_score(int k, double *score,sparse_matrix *sub_matrix, const double *s){/** block 3 **/
    int i;
    for(i=0; i<sub_matrix->col_size; i++){
        if (i == k){
            score[i] = -score[i];
        } else {
            score[i] = score[i] - 4.0*(s[i]*s[k]*calculate_B_ij(sub_matrix,i,k));
        }
    }
}

double *optimize_split(sparse_matrix *sub_matrix, double *s) {
    int i, j, k, *indices, *moved, max_improved_ind_i = 0,j_ind_max_score = 0;
    double k2_m, *improve,*x, delta_q, *score, max_score_j, max_improved_i;
    double *s_old, modularity_before, modularity_after;

    printf("in optimize_split 0,size vectors %d\n", sub_matrix->col_size);

    modularity_before = vector_matrix_vector_product(s, sub_matrix, s);
    printf("------modular before--- %f\n", modularity_before);
    s_old = (double *)malloc(sub_matrix->col_size * sizeof(double));
    memcpy(s_old, s, sizeof(double) * sub_matrix->col_size);


    /** second block once before do **/
    score = (double *) calloc(sub_matrix->col_size, sizeof(double));
    if (score == NULL) {
        failed_alloc("optimize_split");
        return NULL;
    }
    x = (double*) calloc(sub_matrix->col_size, sizeof(double));
    if (x == NULL) {
        failed_alloc("optimize_split\n");
        return NULL;
    }
    x = matrix_vector_product(sub_matrix, s, 0);  /* B */
    for (j = 0; j < sub_matrix->col_size; j++) {
        k2_m =(double)((sub_matrix->nodes_degrees[j])*(sub_matrix->nodes_degrees[j]))/(double)sub_matrix->M;
        score[j] = -2.0*((s[j]*x[j]) + k2_m);
    }
    free(x);

    printf("in optimize_split before do (repeat)\n");
    do {
        moved = (int *)calloc(sub_matrix->col_size, sizeof(int));
        if (moved == NULL){
            failed_alloc("optimize_split");
            return NULL;
        }
        improve = (double *) calloc(sub_matrix->col_size, sizeof(double));
        if (improve == NULL) {
            failed_alloc("optimize_split\n");
            return NULL;
        }
        indices = (int *) calloc(sub_matrix->col_size, sizeof(int));
        if (indices == NULL) {
            failed_alloc("optimize_split\n");
            free(improve);
            return NULL;
        }

        max_improved_i =  -10000000;
        for (i = 0; i < sub_matrix->col_size; i++) { /** line 3 */
            /** calculate maximum score*/
            max_score_j =  -10000000;
            for (j = 0; j < sub_matrix->col_size; j++) {
                if (moved[j])
                    continue;
                if (max_score_j < score[j]) {
                    max_score_j = score[j];
                    j_ind_max_score = j;
                }
            }
            printf("moving vertex with a maximal score: %d\n",j_ind_max_score);

            /** moving vertex with a maximal score */
            s[j_ind_max_score] = -s[j_ind_max_score];
            indices[i] = j_ind_max_score;
            moved[j_ind_max_score] = 1;

            update_score(j_ind_max_score,score,sub_matrix,s);

            if (i == 0) {
                improve[0] = score[j_ind_max_score];
                max_improved_i = improve[0];
                max_improved_ind_i = 0;
            } else {
                /*printf("score[j_ind_max_score] =%f\n",score[j_ind_max_score]);*/
                improve[i] = improve[i-1] + score[j_ind_max_score];
                /*printf("#### improve[%d] is %f\n",i,improve[i]);*/
                if (max_improved_i < improve[i]) { /** line 21- find the maximum improvement of s*/
                    max_improved_i = improve[i];
                    max_improved_ind_i = i;
                }
            }
        }

        for (i = sub_matrix->col_size - 1; i > max_improved_ind_i; --i) {
            j = indices[i];
            s[j] = -s[j];
            update_score(j, score, sub_matrix, s);
        }

        /** line 26 **/
        if (max_improved_ind_i == (sub_matrix->col_size - 1)) {
            delta_q = 0;
        }
        else { delta_q = max_improved_i; }

        free(improve);
        free(moved);
        free(indices);

    } while (delta_q > 0);
    printf("in optimize_split -done!\n");
    free(score);

    modularity_after = vector_matrix_vector_product(s, sub_matrix, s);
    printf("------modular after--- %f\n", modularity_after);
    printf("------modular diff--- %d\n", !(IS_POSITIVE(modularity_after - modularity_before)));

    if (!(IS_POSITIVE(modularity_after - modularity_before))){ /** opt dosn't makes it better, we will use s_old */
        free(s);
        return s_old;
    }
    else{/** we will use s*/
        j = 0;
        k = 0;
        for (i = 0; i < sub_matrix->col_size; i++)
        {
            if(IS_POSITIVE(s[i])){
                j++;
            }
            else{
                k++;
            }
        }
        if((j == 1) || (k == 1))
        {
            free(s);
            return s_old;
        }
        else
        {
            free(s_old);
        }
    }

    return s;
}

double *optimize_split_old(sparse_matrix *sub_matrix, double *s)
{
    int i, j, max_change_idx, changed_max, *moved, *max_improved_s;
    double q_zero, new_score, max_change, temp_improved, max_improved = 0;
    max_improved_s = (int *)malloc(sizeof(int) * sub_matrix->col_size);
    if (max_improved_s == NULL)
    {
        failed_alloc("optimize_split");
        return NULL;
    }
    memcpy(max_improved_s, s, sizeof(int) * sub_matrix->col_size); /* init max_improved_s to s*/
    calc_f_vals(sub_matrix);
    do
    {
        moved = (int *)calloc(sub_matrix->col_size, sizeof(int));
        if (moved == NULL)
        {
            failed_alloc("optimize_split");
            free(max_improved_s);
            return NULL;
        }
        temp_improved = 0;
        for (i = 0; i < sub_matrix->col_size; i++)
        {
            q_zero = vector_matrix_vector_product(s, sub_matrix, s);
            if (q_zero == MALLOC_ERROR)
            {
                free(max_improved_s);
                return NULL;
            }

            changed_max = 0;
            for (j = 0; j < sub_matrix->col_size; j++) /* calculate maximum change with movement of a single node */
            {
                if (moved[i])
                    continue;

                max_improved_s[j] = -max_improved_s[j];
                new_score = vector_matrix_vector_product(s, sub_matrix, s);
                if (new_score == MALLOC_ERROR)
                {
                    free(max_improved_s);
                    return NULL;
                }
                new_score = new_score - q_zero;
                if (changed_max == 0) /* if first- set new max */
                {
                    changed_max = 1;
                    max_change_idx = j;
                    max_change = new_score;
                }
                if (new_score > max_change)  /* if bigger than max so far set new max (equiv. to argmax) */
                {
                    changed_max = 1;
                    max_change_idx = j;
                    max_change += new_score;  /** line 17*/
                }
                max_improved_s[j] = -max_improved_s[j];
            } /* calculated maximum change with movement of a single node max_change_idx */

            temp_improved += max_change;                /* improvement is added */
            moved[max_change_idx] = 1;                  /* mark as moved */
            if (temp_improved > max_improved || i == 0) /* if bigger than max (or first) so far set new max (equiv. to argmax) */
            {
                max_improved_s[max_change_idx] = -max_improved_s[max_change_idx]; /* change maximally improved s */
                if (i == sub_matrix->col_size - 1)
                    max_improved = 0; /* if argmax(improvements) == ng -1 return*/
                else
                    max_improved = temp_improved;
            }
        }
        memcpy(s, max_improved_s, sizeof(int) * sub_matrix->col_size); /* set s to the maximally improved s and redo the loop if needed */
    } while (IS_POSITIVE(max_improved));
    return s;
}
