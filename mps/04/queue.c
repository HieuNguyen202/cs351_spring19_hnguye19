//
// Created by HIEU NGUYEN on 2019-04-02.
//
#include <stdlib.h>
#include <stdio.h>

typedef struct node* nodep_t;
typedef struct node node_t;
typedef struct queue queue_t;
typedef struct queue* queuep_t;

struct node{
    int val;
    struct node* next;
};

struct queue{
    nodep_t front;
    nodep_t back;
    int count;
};

/// Add a node to the back of the queue
/// \param q the queue
/// \param val new value to be added
void enqueue(queuep_t q, int val){
    nodep_t n = malloc(sizeof(node_t));
    n->val = val;
    n->next = NULL;
    if(q->back == NULL){
        q->front = q->back = n;
    } else {
        q->back->next = n;
        q->back = n;
    }
    (q->count)++;
}

/// Remove a node from the front of the queue
/// \param q the queue
/// \return the value of the removed queue
int dequeue(queuep_t q){
    int ret;
    nodep_t n;
    if(q->front == NULL){   //queue is empty
        printf("Queue is empty, it returns -1, but it should raise an exception here!\n");
        return -1;
    }
    n = q->front;
    ret = n->val;
    q->front = q->front->next;      //The next in queue gets to the front
    free(n);                        //Free memory
    q->count--;
    return ret;
}

/// Check if a given value exists in the queue
/// \param q the queue
/// \return 0 id doesn't exist, 1 of does exit
int queue_contains(queuep_t q, int val){
    return 0;
}

/// Initialize a queue
/// \param q pointer to the queue to be initialized
void queue_init(queuep_t q){
    q->count = 0;
    q->front = q->back = NULL;
}

/// Allocate memory for a queue
/// \return pointer to the queue
queuep_t queue_create(){
    queuep_t q = malloc(sizeof(queue_t));
    queue_init(q);
    return q;
}

///Print all values of the queue. Delimited by a space.
void queue_print(queuep_t q){
    nodep_t n = q->front;
    printf(" Queue: ");
    while(n != NULL){
        printf("%d ", n->val);
        n = n->next;
    }
    printf(" count: %d\n", q->count);
}