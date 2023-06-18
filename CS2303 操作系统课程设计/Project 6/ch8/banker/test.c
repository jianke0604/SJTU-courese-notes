#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <fcntl.h>

#define MAX_LINE		80 
int main(int argc, char *argv[])
{
		if (argc != 5) {
        printf("invalid available resource.\n");
        return -1;
    }
    int shouldrun = 1;
    char* input;
    input = (char*) malloc(MAX_LINE * sizeof(char));
    while(shouldrun)
    {
        printf("\033[1;35mosh>\033[0m");
        fflush(stdout);
        fgets(input, MAX_LINE, stdin);
        input[strlen(input) - 1] = '\0';
    }
	return 0;
}