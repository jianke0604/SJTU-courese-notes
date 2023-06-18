/**
 * Simple shell interface program.
 *
 * Operating System Concepts - Tenth Edition
 * Copyright John Wiley & Sons - 2018
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <fcntl.h>


#define MAX_LINE        100 /* 100 chars per line, per command */
int parse(char *input, char* args[]);
void printstatics();
void RL(char* args);
void first_fit(char* name, int size);
void best_fit(char* name, int size);
void worst_fit(char* name, int size);
int memory[256];

int main(int argc, char *argv[])
{
    int shouldrun = 1;
    char* input;
    input = (char*) malloc(MAX_LINE * sizeof(char));
    while(shouldrun)
    {      
        printf("\033[1;35mallocator>\033[0m");
        fflush(stdout);
        fgets(input, 100, stdin);
        input[strlen(input) - 1] = '\0';
        if (strcmp(input, "exit") == 0) {
            shouldrun = 0;
            continue;
        }
        char* args[10];
        for (int i=0; i<10; i++)
        {
            args[i] = (char*) malloc(10 * sizeof(char));
        }
        int arg_num = parse(input, args);
        if (arg_num == 1 && strcmp(args[0], "STAT") == 0)
        {
            printstatics();
        }
        if (arg_num == 4 && strcmp(args[0], "RQ") == 0)
        {
            if (strcmp(args[3], "F") == 0)
                first_fit(args[1], atoi(args[2]));
            if (strcmp(args[3], "B") == 0)
                best_fit(args[1], atoi(args[2]));
            if (strcmp(args[3], "W") == 0)
                worst_fit(args[1], atoi(args[2]));
        }
        if (arg_num == 2 && strcmp(args[0], "RL") == 0)
        {
            RL(args[1]);
        }
    }
	return 0;
}

int parse(char *input, char** args)
{
	int i = 0;
	char* token = strtok(input, " ");
	while (token != NULL) {
		args[i] = token;
		token = strtok(NULL, " ");
		i++;
	}
	args[i] = NULL;
	return i;
}

void first_fit(char* name, int size)
{
    int i = 0;
    char* tmp = (char*) malloc(10 * sizeof(char));
    strcpy(tmp, name+1);
    int pid = atoi(tmp);
    int page = size/4096 + (size%4096 == 0 ? 0 : 1);
    while (i < 256)
    {
        if (memory[i] == 0)
        {
            int j = i;
            while (j < 256 && memory[j] == 0)
            {
                j++;
            }
            if (j - i >= page)
            {
                for (int k=i; k<i+page; k++)
                {
                    memory[k] = pid+1;
                }
                printf("Allocated %s from page %d to page %d\n", name, i, i+page-1);
                return;
            }
            else
            {
                i = j;
            }
        }
        else
        {
            i++;
        }
    }
    printf("Cannot allocate %s, %d\n", name, size);
}

void best_fit(char* name, int size)
{
    int i = 0;
    char* tmp = (char*) malloc(10 * sizeof(char));
    strcpy(tmp, name+1);
    int pid = atoi(tmp);
    int page = size/4096 + (size%4096 == 0 ? 0 : 1);
    int min = 256;
    int min_index = -1;
    while (i < 256)
    {
        if (memory[i] == 0)
        {
            int j = i;
            while (j < 256 && memory[j] == 0)
            {
                j++;
            }
            if (j - i >= page && j - i < min)
            {
                min = j - i;
                min_index = i;
            }
            i = j;
        }
        else
        {
            i++;
        }
    }
    if (min_index == -1)
    {
        printf("Cannot allocate %s, %d\n", name, size);
        return;
    }
    for (int k=min_index; k<min_index+page; k++)
    {
        memory[k] = pid+1;
    }
    printf("Allocated %s from page %d to page %d\n", name, min_index, min_index+page-1);
}

void worst_fit(char* name, int size)
{
    int i = 0;
    char* tmp = (char*) malloc(10 * sizeof(char));
    strcpy(tmp, name+1);
    int pid = atoi(tmp);
    int page = size/4096 + (size%4096 == 0 ? 0 : 1);
    int max = 0;
    int max_index = -1;
    while (i < 256)
    {
        if (memory[i] == 0)
        {
            int j = i;
            while (j < 256 && memory[j] == 0)
            {
                j++;
            }
            if (j - i >= page && j - i > max)
            {
                max = j - i;
                max_index = i;
            }
            i = j;
        }
        else
        {
            i++;
        }
    }
    if (max_index == -1)
    {
        printf("Cannot allocate %s, %d\n", name, size);
        return;
    }
    for (int k=max_index; k<max_index+page; k++)
    {
        memory[k] = pid+1;
    }
    printf("Allocated %s from page %d to page %d\n", name, max_index, max_index+page-1);
}

void RL(char* name)
{
    char* tmp = (char*) malloc(10 * sizeof(char));
    strcpy(tmp, name+1);
    int pid = atoi(tmp);
    int i = 0;
    while (i < 256)
    {
        if (memory[i] == pid+1)
        {
            int j = i;
            while (j < 256 && memory[j] == pid+1)
            {
                j++;
            }
            for (int k=i; k<j; k++)
            {
                memory[k] = 0;
            }
            printf("Process P%d, %d pages, from page %d to page %d is removed from memory.\n", pid, j-i, i, j-1);
            return;
        }
        else
        {
            i++;
        }
    }
    printf("Process %d does not exist.\n", pid);
}

void printstatics()
{
    int i = 0;
    int count = 0;
    while (i < 256)
    {
        int j = i;
        while (j < 256 && memory[j] == memory[i])
        {
            j++;
        }
        if (memory[i] != 0)
        {
            printf("Addresses [%d:%d] Process P%d\n", i*4096, (j-1)*4096+4095, memory[i]-1);
            count++;
        }
        else 
        printf("Addresses [%d:%d] Unused\n", i*4096, (j-1)*4096+4095);
        i = j;
    }
    printf("There are %d process(es) in total\n", count);
}