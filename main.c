// May cause issues compiling if using Musl libc.
// if true, make sure to uncomment this
// #define _GNU_SOURCE // Must have glibc
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define COMMAND_BUFFER_MAX 100
#define ARGC_MAX 32

void tokenizer(char* cmd_buff, char* args[]);

int main(void)
{
    // Init
    char cmd_buff[COMMAND_BUFFER_MAX];

    // Loop
    while (1)
    {
        // Shell prompt
        printf("> ");
        if (fgets(cmd_buff, COMMAND_BUFFER_MAX, stdin) == NULL)
        {
            continue;
        }

        cmd_buff[strlen(cmd_buff) - 1] = '\0';

        // Exit check
        if (strcmp(cmd_buff, "q") == 0)
        {
            return 0;
        }
        // Clear screen check
        if (strcmp(cmd_buff, "clear") == 0)
        {
            printf("\033[H\033[J");
            continue;
        }

        // Tokenization
        char* args[ARGC_MAX] = { NULL };
        tokenizer(cmd_buff, args);

        // Fork processes
        pid_t process = fork();
        if (process == 0)
        {
            // Child
            execvp(args[0], args);

            // if execve fails
            perror(args[0]);
            _exit(EXIT_FAILURE);

            // Preventing the child from becoming a shell
            break;
        }
        else if (process > 0)
        {
            // Parent
            int status;
            waitpid(process, &status, 0);
        }
        else
        {
            // Error
            puts("ERR: fork unsuccessful!");
            return -1;
        }
    }

    return 0;
}

void tokenizer(char* cmd_buff, char* args[])
{
    char* token;

    token = strtok(cmd_buff, " ");

    int counter = 0;
    args[counter] = token;

    while (token != NULL)
    {
        counter++;
        if (counter > ARGC_MAX - 1)
        {
            printf("ERR: argument cap reached: %i!\n", ARGC_MAX);
            exit(EXIT_FAILURE);
        }

        token = strtok(NULL, " ");
        args[counter] = token;
    }
}
