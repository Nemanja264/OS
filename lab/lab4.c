#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <signal.h>
#include <sys/wait.h>

#define N 20

int main()
{
    srand(time(NULL));
    int pid1, pid2;
    int pd1[2], pd2[2];

    if(pipe(pd1) < 0)
        exit(1);

    if(pipe(pd2) < 0)
        exit(1);
    
    pid1 = fork();

    if(pid1 == 0)
    {
        close(pd1[1]);

        char niz[N];
        read(pd1[0], niz, sizeof(char)*N);

        FILE* f = fopen("KARAKTERI.txt", "w");
        if(f == 0)
            exit(2);
        
        for(int i = 0; i<N; i++)
            fprintf(f, "niz[%d] = %c\n", i, niz[i]);
        fclose(f);
        
        close(pd1[0]);
        exit(0);
    }
    else
    {
        pid2 = fork();

        if(pid2 == 0)
        {
            close(pd2[1]); 

            char niz[N];
            read(pd2[0], niz, sizeof(char)*N);
            
            for(int i = 0; i<N; i++)
                printf("niz[%d] = %c\n", i, niz[i]);
            
            close(pd2[0]);
            exit(0);
        }
        else
        {
            close(pd1[0]);
            close(pd2[0]);
           
            char niz[N];
            int zbir = 0;
            for(int i = 0; i<N; i++)
            {
                niz[i] = rand()%58 + 65;
                printf("%c ", niz[i]);
                zbir += niz[i];
            }
            printf("\n");

            if(zbir % 2 == 0)
            {
                write(pd1[1], niz, sizeof(char)*N);
            }
            else
            {
                write(pd2[1], niz, sizeof(char)*N);
            }

            close(pd1[1]);
            close(pd2[1]);
            wait(NULL);
        }

        return 0;
    }   
}
