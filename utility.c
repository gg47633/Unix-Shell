
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <ctype.h>
#include <fcntl.h>
#include "builtIn.h"

#define MAX_COMMAND_LEN 100
#define MAX_ARGUMENTS 100

char command[MAX_COMMAND_LEN];
char *args[MAX_ARGUMENTS];

void runCommands(char *arguments[MAX_ARGUMENTS], int arg_count);
int standardTokenize(char *command);
void redirTokenize(char *command, char ***leftTokens, int *leftCount, char ***rightTokens, int *rightCount);
//Batch input
void batch(char *filename) {
int arg_count;
char **leftTokens;
char **rightTokens;
int leftCount, rightCount;
int stdout_copy = dup(fileno(stdout));
char error_message[30] = "An error has occurred\n";
FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
   }

    while (fgets(command, sizeof(command), file)) {
	char *newline = strchr(command, '\n');
	if(newline) {
		*newline = '\0';
	}
	char *temp = command;
	while(*temp && isspace((unsigned char)*temp)) {
		temp++;
	}
	if(*temp == '\0') { continue; }
	//Tokenization
        int redirCount = 0;
	for(int i = 0; i < strlen(command); i++) {
		if(command[i] == '>') {
			redirCount++;
		}
	}
	if(redirCount == 1) {
		redirTokenize(command, &leftTokens, &leftCount, &rightTokens, &rightCount);
		if(leftCount == 0 || rightCount == 0 || rightCount >= 2) {
			write(STDERR_FILENO, error_message, strlen(error_message));
			exit(0); }
		freopen(rightTokens[0], "w+", stdout);
		runCommands(leftTokens, leftCount);
		fflush(stdout);
		dup2(stdout_copy, fileno(stdout));
		close(stdout_copy);
	} else if(redirCount >= 2) {
		write(STDERR_FILENO, error_message, strlen(error_message));
	} else {
		arg_count = standardTokenize(command);
		//printf("First: %s\n Second %s\n", args[0],args[1]);
		runCommands(args, arg_count);
	}
	}
    fclose(file);
}

void interactive() {
int arg_count;
char **leftTokens;
char **rightTokens;
int leftCount, rightCount;
int stdout_copy = dup(fileno(stdout));
char error_message[300] = "An error has occurred\n";
        int lineCount = 1;
        while (1) {
            printf("wish> ");
            fflush(stdout);

            if (!fgets(command, sizeof(command), stdin)) {
                // If fgets fails, likely due to EOF or error, exit the loop
                exit(0);
            }

            if (strchr(command, '\n')) {
                *strchr(command, '\n') = '\0'; // Remove trailing newline character
            }
		//Tokenization
	 int redirCount = 0;
	 for(int i = 0; i < strlen(command); i++) {
		if(command[i] == '>') {
			redirCount++;
		}
	}
	if(redirCount == 1) {
		redirTokenize(command, &leftTokens, &leftCount, &rightTokens, &rightCount);
		if(leftCount == 0 || rightCount == 0 || rightCount >= 2) {
			write(STDERR_FILENO, error_message, strlen(error_message));
		} else {
		//printf("Left1: %s\n Left2: %s\n Right1: %s\n", leftTokens[0],leftTokens[1],rightTokens[0]);
		freopen(rightTokens[0], "w+", stdout);
		runCommands(leftTokens, leftCount);
		fflush(stdout);
		dup2(stdout_copy, fileno(stdout));
		close(stdout_copy);
		}
	} else if(redirCount >= 2) {
		write(STDERR_FILENO, error_message, strlen(error_message));
	} else {
                arg_count = standardTokenize(command);
		runCommands(args, arg_count);
	}
 }
}           //Commands
void runCommands(char *arguments[MAX_ARGUMENTS], int arg_count) {
	char error_message[100] = "An error has occurred\n";
		//printf("Number of args: %d\n", arg_count);
            if (strlen(command) > 0) {
                if(!strcmp(arguments[0], "cd")) {
			if(arguments[1] == NULL || arg_count > 2) {
			write(STDERR_FILENO, error_message, strlen(error_message));
			} else cd(arguments[1]);
                } else if(!strcmp(arguments[0], "exit")) {
			if(arg_count > 1) {
				write(STDERR_FILENO, error_message, strlen(error_message));
			} else {
			 exit(0);
			}
	        } else if(!strcmp(arguments[0], "path")) {
			path(arguments);
		} else otherCommand(arguments);
            }
}

int standardTokenize(char *command) {
	const char *delimiters = " \t\n\r\f\v";
	char *token = strtok(command, delimiters);
	int arg_count = 0;
	while(token != NULL && arg_count < MAX_ARGUMENTS -1) {
		args[arg_count++] = token;
		token = strtok(NULL, delimiters);
	}
	args[arg_count] = NULL;
	return arg_count;
}

void redirTokenize(char *command, char ***leftTokens, int *leftCount, char ***rightTokens, int *rightCount) {
	char *token, *leftPart, *rightPart;
	const char *delimiters = " \t\n\r\f\v";
	int capacity = 10;

	*leftTokens = malloc(capacity * sizeof(char *));
	*rightTokens = malloc(capacity * sizeof(char *));
	*leftCount = 0;
	*rightCount = 0;

	leftPart = strtok(command, ">");
	rightPart = strtok(NULL, "");

	token = strtok(leftPart, delimiters);
	while(token != NULL) {
		(*leftTokens)[(*leftCount)++] = strdup(token);
		if(*leftCount >= capacity) {
			capacity *= 2;
			*leftTokens = realloc(*leftTokens, capacity * sizeof(char *));
		}
		token = strtok(NULL, delimiters);
	}

	token = strtok(rightPart, delimiters);
	while(token != NULL) {
		(*rightTokens)[(*rightCount)++] = strdup(token);
		if(*rightCount >= capacity) {
			capacity *= 2;
			*rightTokens = realloc(*rightTokens, capacity * sizeof(char *));
		}
		token = strtok(NULL, delimiters);
	}
}
