#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Invalid usage: task needs a tag and a interval.\n");
    } 
    else
    {
        const char* tag = argv[1];
        int interval = atoi(argv[2]);
        while(1)
        {
            printf("\nTask with tag %s which prints every %d seconds", tag, interval);
            sleep(interval);
        }
    }
}