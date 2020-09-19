#include "csr_sparse_matrix.h"
#include "output_and_input.h"
#include "modularity_groups.h"
#include <time.h>
 
int main(int argc, char *argv[]){
    double time_spent = 0.0;
    sparse_matrix *matrix;
    clock_t end, begin;
    queue * division;
    begin = clock();

    if (argc == 3){
        printf("in main- start to initialize\n");
        matrix = initialize_network(argv[1]);
    }
    else{
        printf("Error: wrong number of arguments\n");
        return 1;
    }
    printf("in main- start to split to groups\n");
    division = split_to_groups(matrix);
    printf("in main- start to output\n");
    output_the_division(argv[2], division);
    end = clock();

    time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Total time: %f seconds\n", time_spent);

    return 0;
}
