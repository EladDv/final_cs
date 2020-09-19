#ifndef PROJECT_ERROR_HANDLER_H
#define PROJECT_ERROR_HANDLER_H

#include <stdlib.h>
#include <stdio.h>


/*
*  Error_Handler Summary:
*  A module handling and printing errors and miscellaneous. 
*
*  validate_memory_allocation - Validate that a memory allocation was successful (ptr != NULL). If not, report failure.
*  validate_number_of_elements_returned - Validate that the number of elements returned was as expected. 
*  failed_alloc - Prints error origin.
*  print_vector_double - Prints a double vector.
*  print_vector_int - Prints an integer vector.
*
*/

/*Handles failed dynamic memory allocations.*/
 
/*Validate that a memory allocation was successful (ptr != NULL). If not, report failure. */
void validate_memory_allocation(char *function_name, void *ptr);

/*Validate that the number of elements returned was as expected. */
void validate_number_of_elements_returned(char *function_name, int expected, int observed);

void failed_alloc(char *);

void print_vector_double(double *, int);
void print_vector_int(int *, int);
#endif
