#include "cachelab.h"
#include <stdio.h>
#include <getopt.h>
#include <unistd.h>
#include <stdlib.h>
#include "queue.c"
#define MAX_LINE_LENGTH 16
#define ADDR_SIZE 9

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
unsigned int extract(unsigned int addr, int from, int to);

int main(int argc, char** argv)
{
    res_t res = {0, 0, 0};
    char *optString = "hvs:E:b:t:";
    extern char* optarg;
    extern int optind, opterr, optopt;
    int f_verbose, E, s, b;
    char* trace_file;
    int c;
    char* line;
    char* ret[3];
    FILE* fp;
    size_t len = MAX_LINE_LENGTH;
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
    print_cache(cache);
    while(getline(&line, &len, fp) != -1){
        //ret[0]: access type; ret[1]: address; ret[2]: size
        parse(line, ret);
        if(*ret[0] == 'I')           //ignore instruction fetch
            continue;
        cache_access(cache, hex2bin(ret[1]), &res);

//        printf("%s %s %s\n", ret[0], ret[1], ret[2]);
    }
    //TODO: Make a queue data structure to implement the cache
    //TODO: Run csim-ref with different flags to observe its behaviors
    printSummary(res.hits, res.misses, res.evicts);
    return 0;
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
