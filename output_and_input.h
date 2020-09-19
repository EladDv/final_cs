#ifndef PROJECT_OUTPUT_AND_INPUT_H
#define PROJECT_OUTPUT_AND_INPUT_H


/*
*  Output_and_Input Summary:
*  A module that handles the IO of the program. 
*
*  initialize_network - Initialize network using a given filename
*  output the groups of the division into a given filename
*
*/


#include "queue.h"
#include <stdio.h>
#include <stdlib.h>
#include "csr_sparse_matrix.h"
#include "output_and_input.h"
#include "error_handler.h"

/* Initialize network using a given filename */
sparse_matrix* initialize_network(char *filename);

/* output the groups of the division into a given filename */
void output_the_division(char *filename,queue* division);
 
#endif
