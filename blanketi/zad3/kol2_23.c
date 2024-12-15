#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/types.h>

#define KEY 10302
#define LIMIT 50000
#define TERMINATE 100

struct msgbuf
{
    long mtype;
    int mnum;
};
int main()
{
    srand(time(NULL));
    int pid;
    int qid;
    qid = msgget(KEY, IPC_CREAT|0666);
    struct msgbuf buff;

    pid = fork();
    if(pid == -1)
    {
        perror("Fork failed");
        exit(1);
    }

    if(pid == 0)
    {
        int sum1, sum2, sum3;

        while(1)
        {
            if(msgrcv(qid, &buff, sizeof(buff.mnum), 0, 0) < 0)
            {
                perror("Msgrcv");
                exit(1);
            }

            if(buff.mtype == 1)
            {
                sum1 += buff.mnum;
            }
            else if(buff.mtype == 2)
            {
                sum2 += buff.mnum;
            }
            else
            {
                sum3 += buff.mnum;
            }

            if((sum1 + sum2 + sum3) > LIMIT)
            {
                buff.mnum = 0;
                buff.mtype = TERMINATE;

                if(msgsnd(qid, &buff, sizeof(buff.mnum), 0) < 0)
                {
                    perror("Msgsnd Child");
                    exit(1);
                }
                printf("Final sums: sum1 = %d, sum2 = %d, sum3 = %d\n", sum1, sum2, sum3);
                exit(0);
            }
        } 
    }
    else
    {
        while(1)
        {
            int broj = rand()%1000;
            buff.mnum = broj;

            if(broj < 10)
            {
                buff.mtype = 1;
            }
            else if(broj < 100)
            {
                buff.mtype = 2;
            }
            else
            {
                buff.mtype = 3;
            }

            if(msgsnd(qid, &buff, sizeof(buff.mnum), 0) < 0)
            {
                perror("MsgSnd");
                exit(1);
            }

            if(msgrcv(qid, &buff, sizeof(buff.mnum), TERMINATE, IPC_NOWAIT) != -1)
            {
                printf("Sum of 50 000 has been passed\n");
                break;
            }
        }

        wait(NULL);
        msgctl(qid, IPC_RMID, NULL);
    }

    return 0;
}
