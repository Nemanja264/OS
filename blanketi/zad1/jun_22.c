#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <semaphore.h>

sem_t glavni, sorted;

typedef struct { 
    int* arr;
    int n;
} Array;

void* sort(void* args)
{
    Array* a = (Array*)args;
    sem_wait(&sorted);

    for(int i = 0; i < a->n-1; i++)
        for(int j = 0; j < a->n - i - 1; j++)
            if(a->arr[j] > a->arr[j+1])
            {
                int tmp = a->arr[j];
                a->arr[j] = a->arr[j+1];
                a->arr[j+1] = tmp;
            }

    sem_post(&glavni);
}

int main()
{
    Array a;
    sem_init(&sorted, 0, 0);
    sem_init(&glavni, 0, 0);

    pthread_t t;
    pthread_create(&t, NULL, sort, (void*)&a);

    FILE * f = fopen("ulaz.txt", "r");
    if(f == 0)
        exit(1);
    
    a.n = 0;
    int num;
    a.arr = malloc(100*sizeof(int));
    while(fscanf(f, "%d", &num) == 1)
    {
        a.arr[a.n++] = num;
    }
    fclose(f);

    printf("Unsorted array: ");
    for(int i = 0; i < a.n; i++)
    {
        printf("%d ", a.arr[i]);
    }
    printf("\n");
    sem_post(&sorted);
    sem_wait(&glavni);

    pthread_join(t, NULL);

    printf("Sorted array: ");
    for(int i = 0; i < a.n; i++)
    {
        printf("%d ", a.arr[i]);
    }
    printf("\n");

    sem_destroy(&glavni);
    sem_destroy(&sorted);
    return 0;
}
