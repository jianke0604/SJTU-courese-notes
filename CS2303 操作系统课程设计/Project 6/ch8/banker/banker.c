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
void RQ(char** args);
void RL(char** args);
bool check_safe();

int available[4];
int maximum[5][4];
int need[5][4];
int allocation[5][4];

int main(int argc, char *argv[])
{
	if (argc != 5) {
        printf("invalid available resource.\n");
        return -1;
    }
    int shouldrun = 1;
    char* input;
    input = (char*) malloc(MAX_LINE * sizeof(char));
    available[0] = atoi(argv[1]);
    available[1] = atoi(argv[2]);
    available[2] = atoi(argv[3]);
    available[3] = atoi(argv[4]);
    // freopen("in.txt", "r", stdin);
    // for (int i=0; i<5; i++)
    // {
    //     for (int j=0; j<4; j++)
    //     {
    //         scanf("%d", &maximum[i][j]);
    //         need[i][j] = maximum[i][j];
    //     }
    // }
    // fclose(stdin);
    FILE *fp;
    fp = fopen("input.txt", "r");
    for (int i=0; i<5; i++)
    {
        for (int j=0; j<4; j++)
        {
            fscanf(fp, "%d", &maximum[i][j]);
            need[i][j] = maximum[i][j];
        }
    }
    fclose(fp);
    while(shouldrun)
    {      
        printf("\033[1;35mbanker>\033[0m");
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
        if (arg_num == 1 && strcmp(args[0], "*") == 0)
        {
            printstatics();
        }
        if (arg_num == 6 && strcmp(args[0], "RQ") == 0)
        {
            RQ(args);
        }
        if (arg_num == 6 && strcmp(args[0], "RL") == 0)
        {
            RL(args);
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

void printstatics()
{
    printf("avaliable array is:\n");
    printf("%d %d %d %d\n", available[0], available[1], available[2], available[3]);
    printf("maximum matrix is:\n");
    for (int i = 0; i < 5; i++)
    {
        printf("%d %d %d %d\n", maximum[i][0], maximum[i][1], maximum[i][2], maximum[i][3]);
    }
    printf("allocation matrix is:\n");
    for (int i = 0; i < 5; i++)
    {
        printf("%d %d %d %d\n", allocation[i][0], allocation[i][1], allocation[i][2], allocation[i][3]);
    }
    printf("need matrix is:\n");
    for (int i = 0; i < 5; i++)
    {
        printf("%d %d %d %d\n", need[i][0], need[i][1], need[i][2], need[i][3]);
    }
}

void RQ(char **args)
{
    int pid = atoi(args[1]);
    int request[4];
    request[0] = atoi(args[2]);
    request[1] = atoi(args[3]);
    request[2] = atoi(args[4]);
    request[3] = atoi(args[5]);
    for (int i = 0; i < 4; i++)
    {
        if (request[i] > need[pid][i])
        {
            printf("request is larger than need.\n");
            return;
        }
        if (request[i] > available[i])
        {
            printf("request is larger than available.\n");
            return;
        }
    }
    for (int i = 0; i < 4; i++)
    {
        available[i] -= request[i];
        allocation[pid][i] += request[i];
        need[pid][i] -= request[i];
    }
    if (check_safe())
    {
        printf("\033[32mSuccessfully allocate the resources!\033[0m\n");
    }
    else
    {
        printf("\033[31mThe state is not safe!\033[0m\n");
        for (int i = 0; i < 4; i++)
        {
            available[i] += request[i];
            allocation[pid][i] -= request[i];
            need[pid][i] += request[i];
        }
    }
}

void RL(char **args)
{
    int pid = atoi(args[1]);
    int request[4];
    request[0] = atoi(args[2]);
    request[1] = atoi(args[3]);
    request[2] = atoi(args[4]);
    request[3] = atoi(args[5]);
    for (int i = 0; i < 4; i++)
    {
        if (request[i] > allocation[pid][i])
        {
            printf("\033[31m%d customer doesn't have so many resources!\033[0m\n", pid);
            return;
        }
    }
    for (int i = 0; i < 4; i++)
    {
        available[i] += request[i];
        allocation[pid][i] -= request[i];
        need[pid][i] += request[i];
    }
    printf("\033[32mSuccessfully release the resources!\033[0m\n");
}
bool check_safe()
{
    int work[4];
    bool finish[5];
    for (int i = 0; i < 4; i++)
    {
        work[i] = available[i];
    }
    for (int i = 0; i < 5; i++)
    {
        finish[i] = false;
    }
    int count = 0;
    while (count < 5)
    {
        bool flag = false;
        for (int i = 0; i < 5; i++)
        {
            if (finish[i] == false)
            {
                bool flag2 = true;
                for (int j = 0; j < 4; j++)
                {
                    if (need[i][j] > work[j])
                    {
                        flag2 = false;
                        break;
                    }
                }
                if (flag2)
                {
                    for (int j = 0; j < 4; j++)
                    {
                        work[j] += allocation[i][j];
                    }
                    finish[i] = true;
                    flag = true;
                    count++;
                }
            }
        }
        if (flag == false)
        {
            return false;
        }
    }
    return true;
}