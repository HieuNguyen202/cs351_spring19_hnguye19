#include "cachelab.h"
#include <stdio.h>
#include <getopt.h>
#include <unistd.h>
#include <stdlib.h>

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

int main(int argc, char** argv)
{
    char *optString = "hvs:E:b:t:";
    extern char* optarg;
    extern int optind, opterr, optopt;
    int f_verbose, num_lines_per_set, num_set_index_bits, num_block_bits;
    char* trace_file;
    int c;
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
                num_lines_per_set = atoi(optarg);
                break;
            case 'E':
                //TODO: make sure the input contains valid numeric
                num_lines_per_set = atoi(optarg);
                break;
            case 'b':
                //TODO: make sure the input contains valid numeric
                num_block_bits = atoi(optarg);
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

    //TODO: add command parser
    //TODO: Run csim-ref with different flags to observe its behaviors
    printSummary(0, 0, 0);
    return 0;
}

void print_help(){
    printf("%s", help_message);
}
