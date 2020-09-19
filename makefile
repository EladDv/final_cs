FLAGS = -ansi -Wall -Wextra -Werror -pedantic-errors
OBJS = main.o csr_sparse_matrix.o error_handler.o modularity_groups.o modularity_maximization.o output_and_input.o power_iter.o queue.o

all: $(OBJS)
	gcc $(OBJS) -lm -o cluster

main.o: main.c csr_sparse_matrix.h output_and_input.h modularity_groups.h
	gcc $(FLAGS) -c $*.c 
csr_sparse_matrix.o: csr_sparse_matrix.c csr_sparse_matrix.h error_handler.h
	gcc $(FLAGS) -c $*.c 
error_handler.o: error_handler.c error_handler.h
	gcc $(FLAGS) -c $*.c 
modularity_groups.o: modularity_groups.c modularity_groups.h
	gcc $(FLAGS) -c $*.c 
modularity_maximization.o: modularity_maximization.c modularity_maximization.h error_handler.h
	gcc $(FLAGS) -c $*.c 
output_and_input.o: output_and_input.c csr_sparse_matrix.h output_and_input.h
	gcc $(FLAGS) -c $*.c 
power_iter.o: power_iter.c power_iter.h error_handler.h modularity_maximization.h
	gcc $(FLAGS) -c $*.c -lm
queue.o: queue.c queue.h
	gcc $(FLAGS) -c $*.c
clean:
	rm -rf $(OBJS) cluster
