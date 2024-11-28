#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <semaphore.h>

sem_t upis, sum;
#define N 10

int buff[N];

void* writeOdd(void* args)
{
    for(int i = 0; i<5; i++)
    {
        sem_wait(&upis);
        for(int j=1; j<N; j+=2)
            buff[j] = rand() % 200 + 300;
        sem_post(&sum);
    }
}

void* writeEven(void* args)
{
    for(int i = 0; i<5; i++)
    {
        sem_wait(&upis);
        for(int j=0; j<N; j+=2)
            buff[j] = rand() % 299 + 1;
        sem_post(&sum);
    }
}

int main()
{
    srand(time(NULL));

    sem_init(&upis, 0, 2);
    sem_init(&sum, 0, 0);

    pthread_t t1, t2;
    pthread_create(&t1, NULL, writeEven, NULL);
    pthread_create(&t2, NULL, writeOdd, NULL);

    for(int i = 0; i<5; i++)
    {
        int tzbir = 0;
        sem_wait(&sum);
        sem_wait(&sum);

        for(int j = 0; j<N; j++)
        {
            printf("%d ", buff[j]);
            tzbir += buff[j];
        }
        printf("\n");

        if(tzbir > 2000)
            printf("Zbir zadnjih 10 brojeva je veci od 2000\n");

        printf("Ukupni zbir: %d\n\n", tzbir);

        sem_post(&upis);
        sem_post(&upis);
    }

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    sem_destroy(&upis);
    sem_destroy(&sum);
    return 0;
}
