#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <semaphore.h>

sem_t sedam, ostali;

void* seven(void* args)
{
    int n = * ((int*)args);

    for(int i=7; i<=n; i+=7)
    {
        sem_wait(&sedam);
        printf("%d! ", i);
        sem_post(&ostali);
    }
}

void* rest(void* args)
{
    int n = *((int*)args);

    for(int i = 0; i<=n; i++)
    {
        if(i%7 == 0 && i != 0)
        {
            sem_post(&sedam);
            sem_wait(&ostali);
        }
        else
        {
            sem_wait(&ostali);
            printf("%d ", i);
            sem_post(&ostali);
        }
    }
}

int main(int argc, char* argv[])
{
    int n = atoi(argv[1]);

    sem_init(&sedam, 0, 0);
    sem_init(&ostali, 0, 1);

    pthread_t p1,p2;
    pthread_create(&p1, NULL, (void*)seven, (void*)&n);
    pthread_create(&p2, NULL, (void*)rest, (void*)&n);

    pthread_join(p1, NULL);
    pthread_join(p2, NULL);

    sem_destroy(&sedam);
    sem_destroy(&ostali);

    printf("\n");
    return 0;
}
