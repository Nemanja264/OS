#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <semaphore.h>

sem_t par, nepar, glavni;
char line[100];
int kraj = 0;
int num = 0;

void* parna(void* args)
{
    FILE* p = fopen("parni.txt", "w");
    if(p == 0)
    {
        printf("Fail failure\n");
        exit(1);
    }

    while(1)
    {
        sem_wait(&par);

        if(kraj)
            break;
        
        num++;
        fprintf(p, "%d| %s", num, line);

        sem_post(&glavni);
    }
    fclose(p);
}

void* neparna(void* args)
{
    FILE* p = fopen("neparni.txt", "w");
    if(p == 0)
    {
        printf("Fail failure\n");
        exit(1);
    }

    while(1)
    {
        sem_wait(&nepar);

        if(kraj)
            break;
        
        num++;
        fprintf(p, "%d| %s", num, line);

        sem_post(&glavni);
    }
    fclose(p);
}

int main(int argc, char* argv[])
{
    pthread_t t1, t2;

    sem_init(&par, 0, 0);
    sem_init(&nepar, 0, 0);
    sem_init(&glavni, 0, 1);

    FILE * f = fopen(argv[1], "r");
    if(f == 0)
    {
        printf("Fail failure\n");
        exit(1);
    }

    pthread_create(&t1, NULL, parna,NULL);
    pthread_create(&t2, NULL, neparna,NULL);
    
    int lineNum = 0;
    while(!feof(f))
    {
        sem_wait(&glavni);
        fgets(line, sizeof(line), f);
        lineNum++;

        if(lineNum % 2 == 0)
            sem_post(&par);
        else
            sem_post(&nepar);
    }
    sem_wait(&glavni);
    kraj = 1;

    sem_post(&par);
    sem_post(&nepar);

    fclose(f);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    sem_destroy(&par);
    sem_destroy(&nepar);
    sem_destroy(&glavni);
    
    return 0;
}
