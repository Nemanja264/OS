#include "int.h"
#include <time.h>

int main()
{
    srand(time(NULL));

    int qid = msgget(KEY, IPC_CREAT|0666);
    struct msgbuf buff;
    buff.mtype = 1;

    int pid = fork();
    if(pid == 0)
    {
        int N = rand()%20 + 1;
        printf("N: %d\n\n", N);

        for(int i = 0; i<N; i++){
            buff.mnum = rand()%100 + 1;
            if(msgsnd(qid, &buff, sizeof(int), 0) < 0)
            {
                perror("Msgsnd1");
                exit(1);
            }
        }

        buff.mnum = -1;
        if(msgsnd(qid, &buff, sizeof(int), 0) < 0)
        {
            perror("Msgsnd-1");
            exit(1);
        }

        return 0;
    }

    int odd = 0, even = 0;

    while(1)
    {
        if(msgrcv(qid, &buff, sizeof(int), 0, 0) < 0)
        {
            perror("msgrcv");
            exit(1);
        }

        if(buff.mnum == -1)
            break;

        printf("Broj: %d\n", buff.mnum);

        if(buff.mnum % 2)
            even++;
        else
            odd++;
    }
    printf("\nBroj parnih brojeva: %d\nBroj neparnih brojeva: %d\n", even, odd);

    wait(NULL);
    msgctl(qid, IPC_RMID, NULL);

    return 0;
}