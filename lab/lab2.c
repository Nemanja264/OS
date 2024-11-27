2.
  #include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_COMMAND_LENGTH 1024
#define MAX_ARG_LENGTH 100

void execute_command(char *input) {
    char *args[MAX_ARG_LENGTH];
    char *token = strtok(input, " \t\n");
    int i = 0;
    
    while (token != NULL) {
        args[i++] = token;
        token = strtok(NULL, " \t\n");
    }
    args[i] = NULL;

    // Handle background execution (if the last argument is &)
    int background = 0;
    if (i > 0 && strcmp(args[i - 1], "&") == 0) {
        background = 1;
        args[i - 1] = NULL;  // Remove "&" from arguments
    }

    pid_t pid = fork();
    if (pid == 0) {
        // Child process: execute the command
        execvp(args[0], args);
        // If execvp fails, print an error
        perror("execvp failed");
        exit(1);
    } else if (pid > 0) {
        // Parent process: wait for the child to finish, unless it's background
        if (!background) {
            waitpid(pid, NULL, 0);
        }
    } else {
        perror("Fork failed");
    }
}

void execute_batch_file(const char *filename) {
    FILE *batch_file = fopen(filename, "r");
    if (batch_file == NULL) {
        perror("Error opening batch file");
        return;
    }

    char* line = NULL;
    size_t len = 0;
    while(!feof(batch_file))
      {
        if(getline(&line,&len,batchfile) != -1)
        {
          if(strlen(line)>0)
          {
            printf(Executing: %s\n", line);
            execute(line);
          }
        }
      }
    fclose(batch_file);
}

int main(int argc, char *argv[]) {
    if (argc > 1) {
        // If there's an argument, it might be a batch file
        execute_batch_file(argv[1]);
    } else {
        // Interactive mode: prompt for commands
        char input[MAX_COMMAND_LENGTH];
        while (1) {
            printf("myshell> ");
            if (fgets(input, sizeof(input), stdin) == NULL) {
                break;  // EOF or error
            }

            // Remove newline character
            input[strcspn(input, "\n")] = 0;

            if (strlen(input) == 0) {
                continue;  // Skip empty lines
            }

            if (strcmp(input, "exit") == 0) {
                break; // Exit shell
            }

            execute_command(input);
        }
    }

    return 0;
}
