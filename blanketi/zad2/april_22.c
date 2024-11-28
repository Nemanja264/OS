#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <sys/wait.h>

#define n 10

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
            printf("broj: %d\n", br);
            if(br%3 == 0)
            {
                br += 25;
                write(pd2[1], &br, sizeof(int));
            }
        }

        close(pd1[0]);
        close(pd2[1]);
        wait(NULL);
    }
    else
    {
        close(pd1[0]);
        close(pd2[1]);
        int br;
        for(int i = 0; i<n; i++)
        {
            br = rand()%100 + 200;
            write(pd1[1], &br, sizeof(int));
        }
        close(pd1[1]);

        int divBy3;
        while(read(pd2[0], &divBy3, sizeof(int)))
        {
            printf("Broj deljiv sa 3 pre uvecanja %d, broj posle uvecanja %d\n", divBy3-25, divBy3);
        }
        
        close(pd2[0]);
        wait(NULL);
    }
    return 0;
}
