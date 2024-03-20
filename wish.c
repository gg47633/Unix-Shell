#include "utility.h"
#include "string.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
	char errorMessage[30] = "An error has occurred\n";
	if(argc == 2) {
	if(access(argv[1], F_OK) == 0) {
	batch(argv[1]);
	} else { write(STDERR_FILENO, errorMessage, strlen(errorMessage)); exit(1); }
	} else if(argc == 1) {
	interactive();
	} else { write(STDERR_FILENO, errorMessage, strlen(errorMessage));
		exit(1); }
    return 0;
}
