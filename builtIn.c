#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>

void printError(char *customError);
//"/bin" is first since it's the initial shell path directory
char *pathLen[10] = {"/bin", NULL};

void cd(char *arg) {
   if(chdir(arg) != 0) {
     printError("cd failed");
   }
}

void path(char *args[]) {
   //Empties pathLen
   for(int i = 0; i < 10; i++) {
     pathLen[i] = NULL;
   }
   //Puts values in pathLen except for first argument(the command)
   for(int i = 0; args[i+1] != NULL && i < 9; i++) {
     pathLen[i] = malloc(sizeof(args[i+1]));
     strcpy(pathLen[i], args[i+1]);
     //printf("%s\n", pathLen[i]);
   }
}
void otherCommand(char *args[]) {
   pid_t pid = fork();
   if(pid < 0) {
     printError("Fork failed");
     exit(EXIT_FAILURE);
   } else if (pid == 0) {
     //Child
     if(access(args[0], F_OK | X_OK) == 0) {
       execv(args[0],args);
       printError("Execv failed");
     } else {
       //Iterates through pathLen array
       for(int i = 0; i < 10; i++) {
         char command[1024];
         //Uses snprintf to print formatted string(in path format) to command array
         snprintf(command, sizeof(command), "%s/%s", pathLen[i], args[0]);
         //printf("%s\n", command);
         //Tests if the path in command is accessible
         if(access(command, F_OK | X_OK) == 0) {
           execv(command, args);
           printError("Exec failed here");
         } else {
           if(i == 9) {printError("No File Found");
	   } else { continue;}
	 }
         }
       }
   } else {
     //Parent
     wait(NULL);
   }
}

void printError(char *customError) {
   char error_message[30] = "An error has occurred\n";
   write(STDERR_FILENO, error_message, strlen(error_message));
   //fprintf(stderr, "%s\n", customError);// <- just for testing
   exit(0);
}
