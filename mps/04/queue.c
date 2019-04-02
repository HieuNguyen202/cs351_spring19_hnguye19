//
// Created by HIEU NGUYEN on 2019-04-02.
//
#include <stdlib.h>

typedef struct node* nodep_t;
typedef struct node node_t;
typedef struct queue queue_t;
typedef struct queue* queuep_t;

struct node{
    int val;
    struct node* next;
};

struct queue{
    nodep_t font;
    nodep_t back;
    int count;
};

/// Add a node to the back of the queue
/// \param q the queue
/// \param val new value to be added
void enqueue(queuep_t q, int val){

}

/// Remove a node from the front of the queue
/// \param q the queue
/// \return the value of the removed queue
int dequeue(queuep_t q){

}

/// Check if a given value exists in the queue
/// \param q the queue
/// \return 0 id doesn't exist, 1 of does exit
int queue_contains(queuep_t q, val){

}

/// Initialize a queue
/// \param q pointer to the queue to be initialized
void queue_init(queuep_t q){
    q->count = 0;
    q->back = q->back = NULL;
}

/// Allocate memory for a queue
/// \return pointer to the queue
queuep_t queue_create(){
    queuep_t q = malloc(sizeof(queue_t));
    queue_init(q);
    return &q;
}

int main(int argc, char** argv) {

}
