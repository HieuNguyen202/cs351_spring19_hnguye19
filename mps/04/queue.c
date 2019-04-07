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

typedef struct result res_t;
typedef struct result* resp_t;

struct result{
    int hits, misses, evicts;
};

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
    int s, E, b;
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
    if(q->front == NULL) {          //in case the queue has 1 element before dequeue
        q->back = NULL;
    }
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
    printf("Cache (s = %d, E = %d, b = %d):\n", c->s, c->E, c->b);
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

/// Extract bits from from to to in addr
/// \param addr the address whose bits will be extracted from.
/// \param from from bit index (bit index start from 0 for the least sig. bit (the right most bit)
/// \param to to bit index (inclusive)
/// \return the value of the extracted bits
unsigned int extract(unsigned int addr, int from, int to){
    int l, r;    //left, right
    l = (sizeof(unsigned int) * 8) - 1 - from;
    r = to + l;
    return (addr << l) >> r;
}

void cache_access(cachep_t c, unsigned int addr, resp_t res){
    int sidx, bidx, tag;
    bidx = extract(addr, c->b - 1, 0);
    sidx = extract(addr, c->b + c->s -1, c->b);
    tag = extract(addr, 31, c->b + c->s);
    nodep_t n = c->sets[sidx].lines->front;
    cache_linep_t l, il = NULL;     //cache line, invalid cache line
    while(n != NULL){
        l = (cache_linep_t)n->val;
        if(l->valid){
            if(l->tag == tag){      //hits
                (res->hits)++;
                return;
            }
        } else{                     //Not applied to the implementation of this simulation because the line never becomes invalid. However, if it ever does, this code still work.
            il = l;
        }
        n = n->next;
    }
    //Reaching here means the value is not cached (it's a miss), need to load from lower level memories.
    (res->misses)++;
    if(il != NULL){                 //If a line somehow becomes invalid, simply replace the line with new data
        il->valid = 1;
        il->tag = tag;
        //Should load the blocks to cache (but not applied to this simulation).
        return;
    } else{                         //If there is no invalid lines
        while(c->sets[sidx].lines->count >= c->E){       //While the number of caches line >= the allowed number of cache line
            //evict a line
            dequeue(c->sets[sidx].lines);
            (res->evicts)++;
        }
        cache_linep_t nl = malloc(sizeof(cache_line_t));
        nl->valid = 1;
        nl->tag = tag;
        //Should load the blocks to cache (but not applied to this simulation).
        enqueue(c->sets[sidx].lines, (void*)nl);
    }
    printf("Addr %X tag: %X, sidx: %X, bidx: %X\n", addr, tag, sidx, bidx);
}
