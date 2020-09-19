#include "error_handler.h"


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

void failed_alloc(char *function_name)
{
    printf("Error: function \"%s\" has failed\n", function_name);
}

void print_vector_double(double *v, int length)
{
    int i;
    printf("vector: ");
    for (i = 0; i < length; i++)
    {
        printf("%f,",v[i]);
    }
    printf("\n");
}


void print_vector_int(int *v, int length)
{
    int i;
    printf("vector: ");
    for (i = 0; i < length; i++)
    {
        printf("%d,",v[i]);
    }
    printf("\n");
}

/*
void validate_memory_allocation(char *function_name, void *ptr)
{//check that (ptr != NULL)
    if (ptr == NULL)
    {
        printf("Error: function \"%s\" has failed\n", function_name);
        exit(1);
    }
}
void validate_number_of_elements_returned(char *function_name, int expected, int observed)
{ //check that (expected=observed)
    if (expected != observed)
    {
            printf("Error: function \"%s\" has failed\n", function_name);
        exit(1);
    }
}
*/
