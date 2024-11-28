#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <sys/wait.h>

#define n 5

int main()
{
    int pid;
    int pd1[2], pd2[2];
    srand(time(NULL));

    if(pipe(pd1) < 0)
        exit(1);
    
    if(pipe(pd2) < 0)
        exit(1);

    pid = fork();
    if(pid == -1)
    {
        perror("Fork failed");
        exit(1);
    }
    else if(pid == 0)
    {
        close(pd1[1]);
        close(pd2[0]);

        int br;
        while(read(pd1[0], &br, sizeof(int)))
        {
            if(br%2 == 0 && br != 0)
                write(pd2[1], &br, sizeof(int));
        }

        close(pd1[0]);
        close(pd2[1]);
    }
    else
    {
        close(pd1[0]);
        close(pd2[1]);

        for(int i=0; i<n; i++)
        {
            int br = rand()%100;
            write(pd1[1], &br, sizeof(int));
        }
        close(pd1[1]);

        int paran;
        int brParnih = 0;
        while(read(pd2[0], &paran, sizeof(int)))
        {
            brParnih++;
            printf("%d ", paran);
        }
        printf("\nBroj parnih brojeva je %d\n", brParnih);

        close(pd2[0]);
        wait(NULL);
    }
    return 0;
}
