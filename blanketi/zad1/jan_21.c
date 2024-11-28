#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <semaphore.h>

sem_t glavni, count;

void* countdown(void* args)
{
    char* br = (char*)args;

    while(1)
    {
        sem_wait(&count);
        if(strcmp(br, "KRAJ") == 0)
            break;

        int broj = *br - '0';
        for(int i = broj; i>=0; i--)
        {
            printf("%d\n", i);
            sleep(2);
        }
        
        sem_post(&glavni);
    }

    return 0;
}

int main()
{
    pthread_t t;
    char broj[10];
    sem_init(&glavni, 0, 1);
    sem_init(&count, 0, 0);

    sem_wait(&glavni);
    printf("Unesi broj:");
    scanf("%s", broj);
    sem_post(&count);

    pthread_create(&t, NULL, countdown, (void*)broj);
    
    while(1)
    {
        sem_wait(&glavni);
        printf("Unesi broj:");
        scanf("%s", broj);

        if(strcmp(broj, "KRAJ") == 0)
            break;

        sem_post(&count);
    }
    sem_post(&count);

    pthread_join(t, NULL);
    sem_destroy(&glavni);
    sem_destroy(&count);
    
    return 0;
}
