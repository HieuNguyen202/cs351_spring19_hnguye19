#include "cachelab.h"
#include <stdio.h>
char* help_message = "./csim-ref: Missing required command line argument\n"
                     "Usage: ./csim-ref [-hv] -s <num> -E <num> -b <num> -t <file>\n"
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

int main()
{
    //TODO: add command parser
    print_help();
    printSummary(0, 0, 0);
    return 0;
}

void print_help(){
    printf("%s", help_message);
}
