#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <sys/wait.h>

#define n 100
int main()
{
    char recenica[n];
    printf("Unesi recenicu: ");
    fgets(recenica, sizeof(recenica), stdin);
    recenica[strcspn(recenica, "\n")] = '\0';

    int pid;
    int pd1[2], pd2[2];

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

        char word[n];
        while(read(pd1[0], word, n))
        {
            int sum = 0;
            
            for(int i = 0; i<strlen(word); i++)
                if(isdigit(word[i]))
                    sum += word[i] - '0';

            write(pd2[1], &sum, sizeof(int));
        }

        close(pd1[0]);
        close(pd2[1]);
        wait(NULL);
    }
    else
    {
        close(pd1[0]);
        close(pd2[1]);
        char* word;

        word = strtok(recenica, " ");
        while(word != NULL)
        {
            write(pd1[1], word, strlen(word) + 1);

            int suma;
            read(pd2[0], &suma, sizeof(int));

            printf("Suma cifara u reci \"%s\" je %d\n", word, suma);

            word = strtok(NULL, " ");
            
        }

        close(pd1[1]);
        close(pd2[0]);
        wait(NULL);
    }
    return 0;
}
