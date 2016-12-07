#include <stdio.h>
#include "filestats.h"

int main(int argc, char *argv[])
{
    /* You can modify this file as needed, for example to test different
     * files. If you run on command line, this function optionally reads
     * the filename from command line, if one is included. */
    //char *filename = "testi.txt";
    char *filename = "../rfc791.txt";
    if (argc > 1) {
        filename = argv[1];
    }
    
    printf("Line count: %d\n", line_count(filename));
    
    printf("Word count: %d\n", word_count(filename));
    
    char c = most_common(filename);
    printf("Most common: '%c' (%d)\n", c, c);
}
