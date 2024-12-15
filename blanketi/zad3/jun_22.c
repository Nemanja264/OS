#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <sys/types.h>

#define KEY 10304

struct msgbuf
{
    long mtype;
    int mnum;
};
int main()
{
    int pid;
    int qid;
    qid = msgget(KEY, IPC_CREAT|0666);
    struct msgbuf buff;
    buff.mtype = 1;

    pid = fork();
    if(pid == -1)
    {
        perror("Fork failed");
        exit(1);
    }

    if(pid == 0)
    {
        for(int i = 0; i<10; i++)
        {
            if(msgrcv(qid, &buff, sizeof(buff.mnum), 0, 0) < 0)
            {
                perror("Msgrcv");
                exit(1);
            }
            int sum = 0;
            int cifre = buff.mnum;

            while(cifre > 0)
            {
                sum += cifre%10;
                cifre /= 10;
            }
            printf("%d|suma cifara = %d\n", buff.mnum, sum);
        }
    }
    else
    {
        for(int i = 0; i<10; i++)
        {
            sleep(2);
            int broj;
            printf("Unesi broj:\n");
            scanf("%d", &broj);

            buff.mtype = 1;
            buff.mnum = broj;

            if(broj > 9)
            {
                if(msgsnd(qid, &buff, sizeof(buff.mnum), 0) < 0)
                {
                    perror("Msgsnd");
                    exit(1);
                }
            }
            else
            {
                printf("Unesi visecifreni broj\n");
                i--;
            }
        }
        wait(NULL);
        msgctl(qid, IPC_RMID, NULL);
    }

    return 0;
}
