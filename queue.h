#ifndef QUEUE_H
#define QUEUE_H

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


#include <stdio.h>
#include <stdlib.h>
 
typedef struct node
{
    int *val;
    int *real_nodes;
    int length;
    struct node *next;
} node_t;

typedef struct
{
    node_t *head;
    node_t *tail;
    int length;
} queue;

node_t *init_node();
queue *init_q();

/*void merge_qs(queue *, queue *);*/

node_t *dequeue(queue *);
int enqueue(queue *, int *, int);
int enqueue_node(queue *, node_t *);
void free_q(queue *);

#endif
