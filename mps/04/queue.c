//
// Created by HIEU NGUYEN on 2019-04-02.
//
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


typedef struct node* nodep_t;
typedef struct node node_t;
typedef struct queue queue_t;
typedef struct queue* queuep_t;

typedef struct cache_line cache_line_t;
typedef struct cache_line* cache_linep_t;

typedef struct cache_set cache_set_t;
typedef struct cache_set* cache_setp_t;

typedef struct cache cache_t;
typedef struct cache* cachep_t;

struct node{
    void* val;
    struct node* next;
};

struct queue{
    nodep_t front;
    nodep_t back;
    int count;
};

struct cache_line{
    int valid, tag;
//    void* blocks;   //Pointer to the block array, but not needed for this simulation
};

struct cache_set{
    size_t E;
    queuep_t lines;
};

struct cache{
    size_t s, E, b;
    cache_setp_t sets;
};

/// Add a node to the back of the queue
/// \param q the queue
/// \param val new value to be added
void enqueue(queuep_t q, void* val){
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
void* dequeue(queuep_t q){
    void* ret;
    nodep_t n;
    if(q->front == NULL){   //queue is empty
        return NULL;
    }
    n = q->front;
    ret = n->val;
    q->front = q->front->next;      //The next in queue gets to the front
    free(n->val);
    free(n);                        //Free memory
    q->count--;
    return ret;
}

/// Check if a given value exists in the queue
/// \param q the queue
/// \return 1 if queue contains val, 0 otherwise
int queue_contains(queuep_t q, void* val){
    nodep_t n = q->front;
    while(n != NULL){
        if(strcmp(n->val, (char*)val) == 0){   //identical strings
            return 1;
        }
        n = n->next;
    }
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

///Print all values of the queue. Only used to test the queue. Each node is simply a string. Delimited by a space.
void queue_print(queuep_t q){
    nodep_t n = q->front;
    printf(" Queue: ");
    while(n != NULL){
        printf("%s ", n->val);
        n = n->next;
    }
    printf(" count: %d\n", q->count);
}

void print_cache_line(cache_linep_t l){
    printf("%d %X\n", l->valid, l->tag);
}

///Print all values of the queue. Only used to test the queue. Each node is simply a string. Delimited by a space.
void print_cache_set(cache_setp_t s){
    printf("\tE = %d\n", (int)(s->E));
    nodep_t n = s->lines->front;
    while(n != NULL){
        printf("\tLine: ");
        print_cache_line((cache_linep_t)n->val);
        n = n->next;
    }
}

void print_cache(cachep_t c){
    int S = 1 << (c->s);
    printf("Cache:\n");
    for (int i = 0; i < S; ++i) {
        printf("\tSet %d:\n\t", i);
        print_cache_set(&c->sets[i]);
    }
}

cachep_t make_cache(size_t s, size_t E, size_t b){
    cachep_t c = malloc(sizeof(cache_t));
    //Init sets
    int S = (1<<s);
    cache_setp_t sets = malloc(S * sizeof(cache_set_t));       //array of sets
    for (int i = 0; i < S; ++i) {
        sets[i].E = E;
        sets[i].lines = queue_create();
    }
    //Commit the values
    c->b = b;
    c->E = E;
    c->s = s;
    c->sets = sets;
    return c;
}

void cache_access(int addr, int* hits, int* misses, int* evicts){
    printf("Addr %X\n", addr);
}
