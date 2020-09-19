#include "modularity_groups.h"
/*
*  Modularity_Groups Summary:
*  A module that handles the split of a single graph to modularity groups. 
*
*  split_to_groups - Handles the split of a single graph to modularity groups.
*
*/

queue *split_to_groups(sparse_matrix *start_mod_matrix)
{
    queue *groups_out, *groups_in, *temp_q;
    sparse_matrix *sub_mat;
    node_t *temp_node;
    int *temp, i, k=0, num_isolated=0;

    if (get_c_max(start_mod_matrix) == -1.0)
    {
        return NULL;
    }

    groups_in = init_q();
    if (groups_in == NULL)return NULL;

    groups_out = init_q();
    if (groups_out == NULL)
    {
        free_q(groups_in);
        return NULL;
    }

    for (i = 0; i < start_mod_matrix->col_size; i++)
    {
        if(start_mod_matrix->nodes_degrees[i] == 0)
        {
            temp = (int *)calloc(1, sizeof(int));
            if (temp == NULL)
            {
                free_q(groups_in);
                free_q(groups_out);
                return NULL;
            }
            temp[0] = i;

            temp_node = init_node();
            temp_node->length = 1;
            temp_node->real_nodes = temp;

            temp = (int *)calloc(1, sizeof(int));
            if (temp == NULL)
            {
                free_q(groups_in);
                free_q(groups_out);
                return NULL;
            }
            temp[0] = 0;
            temp_node->val = temp;

            enqueue_node(groups_out, temp_node);
            num_isolated++;
        }

    }


    temp = (int *)calloc(start_mod_matrix->col_size - num_isolated, sizeof(int));
    if (temp == NULL)
    {
        free_q(groups_in);
        free_q(groups_out);
        return NULL;
    }
    for (i = 0; i < start_mod_matrix->col_size; i++)
    {
        if(start_mod_matrix->nodes_degrees[i] != 0){
            temp[k] = i;
            k++;
        }
    }
    printf("node degs: ");
    print_vector_int(start_mod_matrix->nodes_degrees, start_mod_matrix->col_size);
    print_vector_int(temp, k);
    temp_node = init_node();
    temp_node->length = k;
    temp_node->real_nodes = temp;

    temp = (int *)calloc(start_mod_matrix->col_size - num_isolated, sizeof(int));
    if (temp == NULL)
    {
        free_q(groups_in);
        free_q(groups_out);
        return NULL;
    }
    for (i = 0; i < k; i++)
    {
        temp[i] = i;
    }
    temp_node->val = temp;

    if (enqueue_node(groups_in, temp_node) != 0)
    {
        free_q(groups_in);
        free_q(groups_out);
        free(temp);
        return NULL;
    }

    temp = NULL;
    temp_node = NULL;

    while (groups_in->length > 0)
    {
        printf("groups out 0\n");
        temp_node = dequeue(groups_in);
        if (temp_node == NULL)
        {
            free_q(groups_in);
            free_q(groups_out);
        }

        printf("groups out 0a\n");
        printf("starting real_nodes: ");
        print_vector_int(temp_node->real_nodes, temp_node->length);
        sub_mat = get_subset(start_mod_matrix, temp_node->real_nodes, temp_node->length);

        printf("groups out 0c\n");

        if (sub_mat == NULL)
        {
            free_q(groups_in);
            free_q(groups_out);
            return NULL;
        }
        printf("groups out 0d, %d\n", temp_node->length);
        print_vector_int(temp_node->val, temp_node->length);
        temp_q = split_group(sub_mat);

        printf("groups out 0e\n");

        free(temp_node->val);
        free(temp_node);
        temp_node = NULL;



        printf("groups out 1\n");
        if (temp_q->head->length == 0 || temp_q->tail->length == 0)
        {                                  /* if one of them is empty, enqueue to out group the other one */
            if (temp_q->head->length == 0) /* if the head is 0 then remove it then add to out*/
            {
                printf("groups out 2a\n");
                temp_node = dequeue(temp_q);
                if (temp_node != NULL)
                {
                    free(temp_node);
                    temp_node = NULL;
                }
                printf("groups out 2b\n");

            }
            temp_node = dequeue(temp_q);
            if (temp_node == NULL)
            {
                free_q(groups_in);
                free_q(groups_out);
                free_q(temp_q);
                return NULL;
            }
            if (temp_node->length > 0){
                for(i = 0; i < temp_node->length; i++) temp_node->real_nodes[i] = sub_mat->nodes_translation[temp_node->val[i]];
                enqueue_node(groups_out, temp_node);
            }
            free_q(temp_q);
        }
        else
        {
            while (temp_q->length > 0)
            {
                printf("groups out 2c\n");
                temp_node = dequeue(temp_q);
                if (temp_node == NULL)
                {
                    free_q(groups_in);
                    free_q(groups_out);
                    free_q(temp_q);
                    return NULL;
                }
                printf("groups out 2d\n");

                for(i = 0; i < temp_node->length; i++)
                    temp_node->real_nodes[i] = sub_mat->nodes_translation[temp_node->val[i]];

                if (temp_node->length == 1)
                    enqueue_node(groups_out, temp_node); /* if a single node, enqueue to out */
                else
                    enqueue_node(groups_in, temp_node); /* enqueue to in otherwise */
                printf("groups out 2e\n");
            }
        }
        free_sparse_matrix(sub_mat);
        sub_mat = NULL;
    }
    printf("groups out 3\n");
    return groups_out;
}
