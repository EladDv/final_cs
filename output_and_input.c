#include "output_and_input.h"

/*
*  Output_and_Input Summary:
*  A module that handles the IO of the program. 
*
*  initialize_network - Initialize network using a given filename
*  output the groups of the division into a given filename
*
*/
 
sparse_matrix* initialize_network(char *filename) {
    FILE *file;
    int *int_ptr,*col_ind, num_of_nodes, i;
    unsigned int n, degree_of_vi;
    sparse_matrix *matrix = NULL;

    /*The first value of the file represents the number of nodes*/
    int_ptr = (int*) malloc(sizeof(int)*1);
    if(int_ptr == NULL){
        printf("Error: allocate memory for number of nodes has failed\n");
        return NULL;}

    file = fopen(filename, "rb");
    if(file == NULL){
        printf("Error: open input file has failed\n");
        free(int_ptr);
        return NULL;}

    n = fread(int_ptr, sizeof(int), 1, file);
    if (n != 1) {
        printf("Error: read number of nodes from file has failed\n");
        free(int_ptr);
        return NULL;
    }
    num_of_nodes = *int_ptr;
    printf("num of nodes is :%d\n",num_of_nodes);
    matrix = create_sparse_matrix(num_of_nodes, NULL);
    if (matrix == NULL) {
        printf("Error: Creation of matrix failed\n");
        free(int_ptr);
        return NULL;
    }
    /*The next values of the file represent the number of edges of the i'th node, i.e., k_i(degree of i). It is followed by
    the k_i indices of its neighbors, in increasing order.*/
    for (i=0 ; i < num_of_nodes ; ++i) {
        n = fread(int_ptr, sizeof(int), 1, file);
        if (n != 1) {
            printf("Error: read number of edges from file has failed\n");
            free(int_ptr);
            free_sparse_matrix(matrix);
            return NULL;
        }
        degree_of_vi = int_ptr[0];
        matrix->nodes_degrees[i] =(int)degree_of_vi;
        printf("degree of %d is :%d\n",i, degree_of_vi);
        matrix->M += (int)degree_of_vi;

        col_ind = (int*)malloc(degree_of_vi * sizeof(int));
        if(col_ind == NULL){
            printf("Error: allocate memory for neighbors of node has failed\n");
            free(int_ptr);
            free_sparse_matrix(matrix);
            return NULL;}
        n = fread(col_ind, sizeof(int), degree_of_vi, file);
        if (n != degree_of_vi) {
            printf("Error: read number of edges from file has failed\n");
            free(int_ptr);
            free(col_ind);
            free_sparse_matrix(matrix);
            return NULL;
        }
        matrix = add_ones_row(matrix, col_ind, (int)degree_of_vi);
        free(col_ind);
    }
    free(int_ptr);
    return matrix;
}


void output_the_division(char *filename, queue* division) {
    FILE *output_file;
    int i , n, divs;
    node_t * group;

    output_file = fopen(filename, "wb");
    if(output_file == NULL){
        printf("Error: open output file has failed\n");
        return;}

    printf("number of groups: %d \n", division->length);
    /*The first value represents the number of groups in the division.*/
    n = fwrite(&division->length, sizeof(int), 1, output_file);
    if (n != 1) {
        printf("Error: write number of groups to file has failed\n");
        return;
    }
    /*the next values represent the number of nodes in the i'th group, followed by the indices
    of the nodes in the group, in increasing order.**/
    divs = division->length;
    for (i=0 ; i < divs; ++i) {
        group = dequeue(division);
        printf("number of nodes in %d group is: %d\n", i,group->length);
        n = fwrite(&group->length, sizeof(int), 1, output_file);
        if (n != 1) {
            printf("Error: write number of nodes in group to file has failed\n");
            return;
        }
        printf("nodes in %d group are:",i);
        print_vector_int(group->val, group->length);
        /* write the indexes of the nodes in group i */
        n = fwrite(group->real_nodes, sizeof(int),group->length, output_file);
        if (n != group->length) {
            printf("Error: write the indexes of the nodes in group to file has failed\n");
            return;
        }
        printf("output_the_division - finished\n");
    }
}
