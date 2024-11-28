#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    pid_t pid = fork(); // Create a child process

    if (pid < 0) 
    {
        perror("fork");
        return 1;
    } 
    else if (pid == 0) 
    {
        execlp(argv[1], argv[1], argv[2], NULL);

        perror("execlp");
        return 1;
    } 
    else 
    {
        int status;
        wait(&status);
      
        if (WIFEXITED(status)) {
            printf("Child process exited with code: %d\n", WEXITSTATUS(status));
        } else {
            printf("Child process did not terminate normally.\n");
        }
    }

    return 0;
}
