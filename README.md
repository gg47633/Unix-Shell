# Unix-Shell

## Description

This project is a simple recreation of the Unix shell with features such as running Unix commands, redirection, and command parallelization.
- The motivation for creating this shell was to learn how the command line handles and executes certain inputs as well as to become more adept in C
- This shell makes it somewhat easier to understand how exactly the command line works
- This project taught me about forking to multiprocess in C as well as a more in-depth understanding of C as a programming language

## Installation

1. Go to the desired directory you want the repository to be installed. This can be done in the terminal:
##
    cd <next_location>
2. Once you have reached the desired directory, run this command to clone the repository into your directory:
##
    git clone https://github.com/gg47633/Unix-Shell.git
3. If needed, change directory into the repository to access the code:
##
    cd Unix_Shell

## Usage

1. Once in the Unix-Shell directory, run this command to compile the code:
##
    make
2. To begin interactive mode, run this command:
##
    ./wish
3. To use batch mode with an already existing file with commands, run this command:
##
    ./wish <file>
4. While in interactive mode, you can run normal shell commands such as ls, cd, mkdir, etc. as well as redirection (> operand) or run commands in parallel (& operand)
5. To use redirection (changing where a command outputs to), use this command template:
##
    command > output_location
6. To run commands in parallel (more than two commands can be run in parallel at once), use this command template:
##
    command1 & command2 & command3
7. To leave interactive mode:
##
    exit
