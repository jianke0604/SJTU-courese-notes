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

#define MAX_LINE		80 /* 80 chars per line, per command */
int parse(char *input, char* args[]);
bool check_concurrent(char *input);

int main(void)
{
		char *args[MAX_LINE/2 + 1];	/* command line (of 80) has max of 40 arguments */
    	int should_run = 1;
		bool have_history = false;
		char* input;
		char* last_input;
		char* in_file;
		char* out_file;
		bool concurrent = false;
		pid_t pid;

		input = (char*) malloc(MAX_LINE * sizeof(char));
		last_input = (char*) malloc(MAX_LINE * sizeof(char));
		in_file = (char*) malloc(MAX_LINE * sizeof(char));
		out_file = (char*) malloc(MAX_LINE * sizeof(char));
    	while (should_run){   
        	printf("\033[1;35mosh>\033[0m");
        	fflush(stdout);
			fgets(input, MAX_LINE, stdin);
			input[strlen(input) - 1] = '\0';
			concurrent = check_concurrent(input);
			// printf("%s", input);
			// printf("%d\n", strcmp(input, "exit"));
			if (strcmp(input, "exit") == 0) {
				should_run = 0;
				continue;
			}

			// history
			if (strcmp(input, "!!") == 0) {
				if (have_history) {
					printf("%s\n", last_input);
					strcpy(input, last_input);
				}
				else {
					printf("No commands in history.\n");
					continue;
				}
			}
			else{
				strcpy(last_input, input);
				// printf("%d\n",strcmp(input, "!!") == 0);
				// printf("%s\n", last_input);
				have_history = true;
			}
			

			pid = fork();
			if (pid < 0) printf("Error: Fork failed!\n");
			else
			{
				if (pid==0)
				{
					for (int i = 0; i <= MAX_LINE / 2; ++ i) 
						args[i] = (char*) malloc(MAX_LINE * sizeof(char));
			
					// printf("test\n");
					int cnt = parse(input, args);
					bool error_flag = false;
					// printf("%d\n", cnt);
					// for (int i = 0; i < n; i++) {
					// 	printf("%s\n", args[i]);
					// }
					// printf("now\n");
					// wait(NULL);
					for (int i = cnt; i <= MAX_LINE / 2; ++ i) 
					{
						free(args[i]);
						args[i] = NULL;
					}
					if (concurrent) 
					{
						// free(args[cnt - 1]);
						args[cnt - 1] = NULL;
						cnt --;
					}
					// find pipe
					int pipe_pos = -1;
					for (int i = 0; i < cnt; ++ i)
						if (strcmp(args[i], "|") == 0) {
							pipe_pos = i;
							break;
						}
					if (pipe_pos >=0)
					{
						if (pipe_pos == 0 || pipe_pos == cnt - 1) 
						{
							printf("Error: Unexpected syntax '|'.\n");
							error_flag = 1;
						}
						if (error_flag) continue;
						int fd[2];
						pipe(fd);
						pid_t pid1 = fork();
						if (pid1 < 0) printf("Error: Fork failed!\n");
						else if (pid1 == 0)
						{
							for (int i=pipe_pos; i<cnt; i++)
								args[i] = NULL;
							close(fd[0]);
							dup2(fd[1], STDOUT_FILENO);
							execvp(args[0], args);
							close(fd[1]);
						}
						else
						{
							wait(NULL);
							for (int i = pipe_pos + 1; i < cnt; ++ i) args[i - pipe_pos - 1] = args[i];
							for (int i = cnt - pipe_pos - 1; i < cnt; ++ i) args[i] = NULL;
							close(fd[1]);
							dup2(fd[0], STDIN_FILENO);
							close(fd[0]);
							execvp(args[0], args);
						}
					}
					else
					{
						int in_redirect = 0, out_redirect = 0;
						if (cnt >= 3 && (strcmp(args[cnt - 2], ">") == 0 || strcmp(args[cnt - 2], "<") == 0)) 
						{
							if (strcmp(args[cnt - 2], ">") == 0) 
							{
								out_redirect = 1;
								strcpy(out_file, args[cnt - 1]);
							}
							else 
							{
								in_redirect = 1;
								strcpy(in_file, args[cnt - 1]);
							}
							args[cnt - 2] = NULL;
							args[cnt - 1] = NULL;
							free(args[cnt - 2]);
							free(args[cnt - 1]);
							cnt -= 2;
						}
						if (in_redirect) {
							int fd = open(in_file, O_RDONLY);
							if (fd < 0) {
								printf("Error: Cannot open file %s.\n", in_file);
								error_flag = 1;
							}
							if (error_flag) continue;
							dup2(fd, STDIN_FILENO);
							close(fd);
						}
						if (out_redirect) {
							int fd = open(out_file, O_WRONLY | O_CREAT | O_TRUNC, 0666);
							if (fd < 0) {
								printf("Error: Cannot open file %s.\n", out_file);
								error_flag = 1;
							}
							if (error_flag) continue;
							dup2(fd, STDOUT_FILENO);
							execvp(args[0], args);
							close(fd);
						}
						execvp(args[0], args);
					}
					free(in_file);
					free(out_file);
					for (int i=0; i<cnt; i++)
						free(args[i]);
				}
				else if(!concurrent)
					wait(NULL);
			}
        	/**
         	 * After reading user input, the steps are:
         	 * (1) fork a child process
         	 * (2) the child process will invoke execvp()
         	 * (3) if command includes &, parent and child will run concurrently
         	 */
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

bool check_concurrent(char *input) {
	int len = strlen(input);
	if(len && input[len - 1] == '&') return 1;
	return 0;
}