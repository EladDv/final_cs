
#ifndef MODULARITY_GROUPS_H
#define MODULARITY_GROUPS_H

/*
*  Modularity_Groups Summary:
*  A module that handles the split of a single graph to modularity groups. 
*
*  split_to_groups - Handles the split of a single graph to modularity groups.
*
*/


#include "modularity_maximization.h"
#include "queue.h"

queue *split_to_groups(sparse_matrix *);
 
#endif
