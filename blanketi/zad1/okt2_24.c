#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <semaphore.h>

sem_t par, nepar;
int over = 0;

struct data
{
    int br;
    char* ime;
    FILE* file;
};

void* odd(void* args)
{
    struct data * d = (struct data *)args;

    FILE * f = fopen(d->ime, "r");
    if(f == NULL)
    {
        printf("File opening failed\n");
        exit(1);
    }
    char line[100];

    while(!feof(f) && !over)
    {
        fgets(line, sizeof(line), f);
        d->br++;

        sem_wait(&nepar);
        fprintf(d->file, "%s|%d %s", d->ime, d->br, line);
        sem_post(&par);
    }

    fclose(f);
    over = 1;
    return NULL;
}

void* even(void* args)
{
    struct data * d = (struct data *)args;
    FILE * f = fopen(d->ime, "r");
    if(f == NULL)
    {
        printf("File opening failed\n");
        exit(1);
    }
    char line[100];

    while(!feof(f) && !over)
    {
        fgets(line, sizeof(line), f);
        d->br++;

        sem_wait(&par);
        fprintf(d->file, "%s|%d %s", d->ime, d->br, line);
        sem_post(&nepar);
    }

    fclose(f);
    over = 1;
    return NULL;
}

int main(int argc, char* argv[])
{
    pthread_t p, n;
    FILE * z = fopen("zbir.txt", "w");
    if(z == NULL)
    {
        printf("File opening failed\n");
        return 1;
    }

    struct data deven;
    deven.ime = (char*)malloc((strlen(argv[1]+1)*sizeof(char)));
    strcpy(deven.ime, argv[1]);
    deven.br = 0;
    deven.file = z;

    struct data dodd;
    dodd.ime = (char*)malloc((strlen(argv[2]+1)*sizeof(char)));
    strcpy(dodd.ime, argv[2]);
    dodd.br = 0;
    dodd.file = z;

    sem_init(&par, 0, 0);
    sem_init(&nepar, 0, 1);

    pthread_create(&p, NULL, (void*)even, (void*)&deven);
    pthread_create(&n, NULL, (void*)odd, (void*)&dodd);

    pthread_join(p, NULL);
    pthread_join(n, NULL);

    fclose(z);

    sem_destroy(&par);
    sem_destroy(&nepar);

    free(dodd.ime);
    free(deven.ime);
    return 0;
}
