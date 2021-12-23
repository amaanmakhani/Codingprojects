#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "coding2.h"

#define MAX_FILENAME_LEN 256

void add_filename_extension(char *startname, char *endname, char *suffix)
{
    char temp[MAX_FILENAME_LEN];
    char *dot, *slash;

    strncpy(temp, startname, MAX_FILENAME_LEN-1);
    dot = strrchr(temp, '.');
    slash = strrchr(temp, '/');

    /*
     * Must check the tricky case "/somedir/otherdir.foo/bar"
     */
    if (dot && (slash == NULL || slash < dot)) {
        *dot = '\0';
    }
    snprintf(endname, MAX_FILENAME_LEN-1, "%s.%s", temp, suffix);
       
    return; 
}


int main(int argc, char *argv[]) {
    char *infile_name;
    char outfile_name[MAX_FILENAME_LEN];
    FILE *input = NULL;
    FILE *output = NULL;
    
    if (argc < 2) {
        fprintf(stderr, "usage: %s <filename>\n", argv[0]);
        exit(1);
    } else {
        infile_name = argv[1];
    }

    add_filename_extension(infile_name, outfile_name, "mtf");

    input = fopen(infile_name, "r");
    if (input == NULL) {
        fprintf(stderr, "error: cannot open %s for input\n",
            infile_name);
        exit(2);

    }

    output = fopen(outfile_name, "w");
    if (output == NULL) {
        fprintf(stderr, "error: cannot open %s for output\n",
            outfile_name);
        exit(2);
    }

    assert (input && output);

    if (encode(input, output) == 0) {
        fclose(input);
        fflush(output);
        fclose(output);
    } else {
        fprintf(stderr, "Encode failed\n");
        fclose(input);
        fclose(output);
        remove(outfile_name);
    }
}
