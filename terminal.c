/*
A very simple terminal emulator written in C.
1. Accepts user input.
2. Converts to tokens.
3. Makes relevant system call using exec commmand.
4. Flushes output.
*/

#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>
#include<wait.h>


// macro definitions
#define MAX_LEN 100
#define MAX_WORDS 100


char** split_commands(char* command) {
    char** result=malloc(MAX_WORDS * sizeof(char*));

    char* token=strtok(command, " ");

    int i=0;
    while(token && i<=MAX_LEN) {
        result[i]=token;
        token=strtok(NULL, " ");
        i++;
    }
    result[i]=NULL;
    return result;
}


void print_tokens(char** tokens) {
    for(int i=0; tokens[i]; i++) {
        printf("%s\n", tokens[i]);
    }
}


void execute_command(char** args) {
    // create child process using fork
    // use execvp to execute commands

    pid_t pid=fork();

    if(pid == 0) {
        // child process
        int stat=execvp(args[0], args);
        if(stat == -1) {
            printf("Error while executing exec command\n");
            exit(1);
        }
    } else if(pid > 0) {
        // parent process (wait till child process completes execution)
        wait(NULL);
    } else {
        printf("Error while creating child process\n");
    }
}


void parse_tokens(char** tokens) {
    char* first_token=tokens[0];

    if(strcmp(first_token, "exit") == 0) {
        printf("Goodbye!\n");
        exit(0);
    } else if (strcmp(first_token, "clear") == 0) {
        system("clear");
    }

    // call execute_command
    execute_command(tokens);
}


void init() {
    printf("Welcome\n");
    printf("%s\n\n", __DATE__);
    printf("=== Commands supported ===\n");
    printf("1. common UNIX commands\n");
    printf("2. exit  (to exit terminal session)\n");
    printf("3. clear (to clear terminal)\n");
    printf("4. pipe commands (under development)\n\n");
}


void main(int argc, char* argv) {

    init();
    int cmd_status=1;

    // RPEL strategy
    do {
        char input_buffer[MAX_LEN];
        printf("$ ");
        fgets(input_buffer, MAX_LEN, stdin);
        
        // remove newline
        input_buffer[strcspn(input_buffer, "\n")] = '\0';

        // accept input
        char** tokens=split_commands(input_buffer);

        // print_tokens(tokens);
        parse_tokens(tokens);
        
        // free memory
        free(tokens);

        printf("freed memory\n");
    }
    while (cmd_status == 1);
}
