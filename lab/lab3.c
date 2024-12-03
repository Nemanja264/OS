#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

sem_t vrsta, kolona;
#define N 3

struct matrix
{
    int mat[N][N];
};

void* rows(void* args)
{
    struct matrix * r = (struct matrix*)args;

    for(int k = 0; k < N; k++)
    {
        sem_wait(&vrsta);
        for(int i = 0; i<N; i++)
        {
            for(int j = 0; j<N-i-1; j++)
            {
                if(r->mat[k][j] > r->mat[k][j+1])
                {
                    int temp = r->mat[k][j];
                    r->mat[k][j] = r->mat[k][j+1];
                    r->mat[k][j+1] = temp;
                }
            }
        }
        sem_post(&kolona);
    }
    return NULL;
}

void* columns(void* args)
{
    struct matrix * r = (struct matrix*)args;

    for(int k = 0; k < N; k++)
    {
        sem_wait(&kolona);
        for(int i = 0; i<N; i++)
        {
            for(int j = 0; j<N-i-1; j++)
            {
                if(r->mat[j][k] > r->mat[j+1][k])
                {
                    int temp = r->mat[j][k];
                    r->mat[j][k] = r->mat[j+1][k];
                    r->mat[j+1][k] = temp;
                }
            }
        }
        sem_post(&vrsta);
    }
    return NULL;
}

int main()
{
    struct matrix m;
    printf("Unesi matricu:\n");
    for(int i = 0; i<N; i++){
        printf("%d. vrsta:\n", i+1);
        for(int j = 0; j<N; j++)
            scanf("%d", &(m.mat[i][j])); 
    }
    printf("UNETA MATRICA!\n");

    sem_init(&vrsta, 0, 0);
    sem_init(&kolona, 0, 1);

    pthread_t t1,t2;
    pthread_create(&t1, NULL, (void*)rows, (void*)&m);
    pthread_create(&t2, NULL, (void*)columns, (void*)&m);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    sem_destroy(&vrsta);
    sem_destroy(&kolona);

    printf("\nSorted Matrix:\n");
    for(int i = 0; i<N; i++){
        printf("%d. vrsta: ", i+1);
        for(int j = 0; j<N; j++)
            printf("%d ", m.mat[i][j]); 
        printf("\n");
    }

    return 0;
}
