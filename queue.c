#include "queue.h"


/*
*  Queue Summary:
*  A structure that implements a queue data structure for holding groups of graph subsets. 
*
*  init_node - initializes a node that holds an array of graph node indicies, their translation to real nodes in the original graph and next node.
*  init_q - initializes an empty queue of nodes.
*  dequeue - removes the first item on the queue and returns it.
*  enqueue - creates a new node and enqueues it.
*  enqueue_node - enqueues an existing node to a queue.
*  free_q - frees a queue and all associated nodes in it.
*
*/


node_t *init_node()
{ 
    node_t *node;
    node = (node_t *)malloc(sizeof(node_t));
    if (node == NULL)
        return NULL;

    node->length = -1;
    node->val = NULL;
    node->real_nodes = NULL;
    node->next = NULL;
    return node;
}

queue *init_q()
{
    queue *q;
    q = (queue *)malloc(sizeof(queue));
    if (q == NULL)
        return NULL;
    q->length = 0;
    q->head = NULL;
    q->tail = NULL;
    return q;
}

/*void merge_qs(queue *q1, queue *q2)
{
    *q1->length = q1->length + *q2->length;
    if (q1->head != NULL)
        q1->tail->next = q2->head;
    if (q1->head == NULL)
        q1->head = q2->head;
    q1->tail = q2->tail;
    free(q2);
}
*/
int enqueue_node(queue *q, node_t *tmp)
{
    if (q->tail != NULL)
        q->tail->next = tmp;
    q->tail = tmp;
    if (q->head == NULL)
        q->head = tmp;
    (q->length)++;
    return 0;
}
int enqueue(queue *q, int *x, int length_x)
{
    node_t *tmp;
    int i;

    tmp = init_node();
    if (tmp == NULL)
        return 1;

    tmp->val = x;
    tmp->length = length_x;
    if(length_x > 0){
        tmp->real_nodes = (int *)calloc(length_x, sizeof(int));
        if(tmp->real_nodes == NULL)
        {
            free(tmp);
            return 1;
        }
        for(i = 0; i < tmp->length; i++) tmp->real_nodes[i] = i;

    }
    if (q->tail != NULL)
        q->tail->next = tmp;
    q->tail = tmp;
    if (q->head == NULL)
        q->head = tmp;
    (q->length)++;
    return 0;
}

node_t *dequeue(queue *q)
{
    node_t *tmp;
    if (q->length <= 0)
        return NULL;
    else
    {
        tmp = q->head;

        q->head = tmp->next;
        if (tmp->next == NULL)
            q->tail = NULL;
        q->length = q->length-1;

        tmp->next = NULL;
        return tmp;
    }
}

void free_q(queue *q)
{
    node_t *tmp;
    while (q->length > 0)
    {
        tmp = dequeue(q);
        if (tmp != NULL)
        {
            free(tmp->val);
            if(tmp->real_nodes != NULL) free(tmp->real_nodes);
            free(tmp);
        }
    }
    free(q);
}
