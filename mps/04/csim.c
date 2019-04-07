#include "cachelab.h"
#include <stdio.h>
#include <getopt.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_LINE_LENGTH 16
//#define ADDR_SIZE 9

char* help_message = "Usage: ./csim [-hv] -s <num> -E <num> -b <num> -t <file>\n"
                     "Options:\n"
                     "  -h         Print this help message.\n"
                     "  -v         Optional verbose flag.\n"
                     "  -s <num>   Number of set index bits.\n"
                     "  -E <num>   Number of lines per set.\n"
                     "  -b <num>   Number of block offset bits.\n"
                     "  -t <file>  Trace file.\n"
                     "\n"
                     "Examples:\n"
                     "  linux>  ./csim-ref -s 4 -E 1 -b 4 -t traces/yi.trace\n"
                     "  linux>  ./csim-ref -v -s 8 -E 2 -b 4 -t traces/yi.trace";
void print_help();  //Print help message
void parse(char* buf, char** ret);
unsigned int hex2bin(char* hex);
unsigned int extract(unsigned long addr, int from, int to);


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
    int valid;
    unsigned long tag;
    clock_t time;    //last accessed time
//    void* blocks;   //Pointer to the block array, but not needed for this simulation
};

struct cache_set{
    size_t E;
    cache_linep_t lines;
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
        printf("%s ", (char*)(n->val));
        n = n->next;
    }
    printf(" count: %d\n", q->count);
}

void print_cache_line(cache_linep_t l){
    printf("Valid tag time: %d %lx %lu\n", l->valid, l->tag, l->time);
}

///Print all values of the queue. Only used to test the queue. Each node is simply a string. Delimited by a space.
void print_cache_set(cache_setp_t s){
    printf("\tE = %d\n", (int)(s->E));
    for (int i = 0; i < s->E; ++i) {
        printf("\t\tLine: ");
        print_cache_line(&(s->lines[i]));
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
        sets[i].lines = malloc(E * sizeof(cache_line_t));      //array of lines
        for (int j = 0; j < E; ++j) {
            sets[i].lines[j].valid = 0;
        }
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
unsigned int extract(unsigned long addr, int from, int to){
    int l, r;    //left, right
    l = (sizeof(addr) * 8) - 1 - from;
    r = to + l;
    return (addr << l) >> r;
}

void cache_access(cachep_t c, unsigned long addr, resp_t res, int f_verbose){
    int sidx, tag;
//    bidx = extract(addr, c->b - 1, 0);
    sidx = extract(addr, c->b + c->s -1, c->b);
    if(f_verbose){  //trying to trick the compiler that i'm suing f_verbose
        printf(" set %d", sidx);
    }
    tag = extract(addr, (sizeof(unsigned long) * 8) - 1, c->b + c->s);
    cache_setp_t set = &(c->sets[sidx]);
    cache_linep_t il = NULL;    //Invalid line
    for (int i = 0; i < set->E; ++i) {
        cache_linep_t l = &(set->lines[i]);
        if (l->valid) {
            if (l->tag == tag) {
                (res->hits)++;      //hit
                l->time = clock();
                if(f_verbose){  //trying to trick the compiler that i'm suing f_verbose
                    printf(" hit\n");
                }
                return;
            }
        } else {
            il = l;
        }
    }
    (res->misses)++;                //miss
    if(f_verbose){
        printf(" miss");
    }
    if(il != NULL) {                //Still have empty (invalid) lines
        il->valid = 1;
        il->tag = tag;
        il->time = clock();
        if(f_verbose){  //trying to trick the compiler that i'm suing f_verbose
            printf("\n");
        }
        return;
    }
                                    //eviction
    cache_linep_t ll = set->lines;  //Least recently accessed line, first line by default
    for (int i = 0; i < set->E; ++i) {
        cache_linep_t l = &(set->lines[i]);
        if(l->time < ll->time) {
            ll = l;
        }
    }
    if(f_verbose){  //trying to trick the compiler that i'm suing f_verbose
        printf(" eviction\n");
    }
    ll->valid = 1;                  //Maybe redundant because it's always valid when reach here.
    ll->tag = tag;
    ll->time = clock();
    (res->evicts)++;
}

/// Convert ASCII encoded HEX to binary
/// \param hex ASCII encoded HEX
/// \return equivalent binary
unsigned int hex2bin(char* hex){
    unsigned int d, ret = 0;
    int len = strlen(hex);
    char c;
    for (int i = len -1; i > -1; --i) {
        c = hex[i];
        //Convert the char from ASCII to binary
        if(c>= '0' && c<= '9'){
            d = c - '0';
        } else if(c >= 'A' && c <= 'F'){
            d = c - 'A' + 10;
        } else if(c>= 'a' && c<= 'f') {
            d = c - 'a' + 10;
        } else {
            printf("hex2bin: invalid hex is given '%c' in '%s'\n", c, hex);
        }
        //Shift the bits to the right position
        d = d << ((len - 1 - i)*4);
        //OR with the accumulator
        ret |= d;
    }
    return ret;
}

/// Parse a line. The ret contains null terminated pieces of info of the line:
/// \param buf the input line
/// \param ret the result
/// ret[0] pointer to the operation char
/// ret[1] pointer to the address
/// ret[2] pointer to the size
void parse(char* buf, char** ret){
    while(*buf == ' ') buf++;   //Skip leading white spaces
    ret[0] = buf;               //Set the pointer of the operation char
    buf++;                      //Next char
    *buf = '\0';                //Set current char to '\0' (it should have been a space char before)
    buf++;                      //Next char
    while(*buf == ' ') buf++;   //Skip white spaces
    ret[1] = buf;               //Set the pointer for the address
    while(*buf != ',') buf++;   //Skip until a comma
    *buf = '\0';                //Change the comma to '\0'
    ret[2] = ++buf;             //Set the pointer for size
    while(*buf != '\n') buf++;  //Skip until a new line char
    *buf = '\0';                //Change the new line char to '\0'
}

///Print a help message
void print_help(){
    printf("%s", help_message);
}

int main(int argc, char** argv)
{
    res_t res = {0, 0, 0};
    char *optString = "hvs:E:b:t:";
    extern char* optarg;
    extern int optind, opterr, optopt;
    int f_verbose = 0, E, s, b;
    char* trace_file;
    int c;
    char line[MAX_LINE_LENGTH];
    char* ret[3];
    FILE* fp;
    while ((c=getopt(argc, argv, optString))!=-1){
        switch(c){
            case 'h':
                print_help();
                exit(0);
            case 'v':
                f_verbose = 1;
                break;
            case 's':
                //TODO: make sure the input contains valid numeric
                s = atoi(optarg);
                break;
            case 'E':
                //TODO: make sure the input contains valid numeric
                E = atoi(optarg);
                break;
            case 'b':
                //TODO: make sure the input contains valid numeric
                b = atoi(optarg);
                break;
            case 't':
                trace_file = optarg;
                break;
            default:    //'?'
                //TODO: devide what to do by default
                printf("./csim: Missing required command line argument\n");
                print_help();
                exit(0);
        }
    }
    if((fp = fopen(trace_file, "r")) == NULL){
        printf("Failed to open file %s\n", "traces/yi.trace");
        exit(1);
    }
    cachep_t cache = make_cache(s, E, b);      //s, E, b
    while(fgets(line, MAX_LINE_LENGTH, fp) != NULL){
        //ret[0]: access type; ret[1]: address; ret[2]: size
        parse(line, ret);
        if(*ret[0] == 'I')           //ignore instruction fetch
            continue;
        if(f_verbose){
            printf("%s %s,%s", ret[0], ret[1], ret[2]);
        }
        cache_access(cache, hex2bin(ret[1]), &res, f_verbose);
        if(*ret[0] == 'M')           //Modify operation access cache twice
            cache_access(cache, hex2bin(ret[1]), &res, f_verbose);
    }
    printSummary(res.hits, res.misses, res.evicts);
    return 0;
}
