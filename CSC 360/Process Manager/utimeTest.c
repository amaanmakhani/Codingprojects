#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Invalid usage: task needs a interval.\n");
    } 
    else
    {
        int interval = atoi(argv[1]);
        while(1)
        {
            usleep(interval);
        }
    }
}