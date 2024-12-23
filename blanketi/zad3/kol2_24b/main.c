#include "int.h"

#define len 256

int main()
{
    struct msgbuf buff;

    int qid = msgget(KEY, IPC_CREAT|0666);

    int pid = fork();
    if(pid == -1)
    {
        perror("Fork failed");
        exit(1);
    }

    if(pid == 0)
    {
        FILE* f = fopen("prvabr.txt", "r");
        if(f == NULL)
        {
            perror("File failure");
            exit(1);
        }
        
        char br[len];
        while(fgets(br, len, f))
        {
            buff.mnum = atoi(br);
            buff.mtype = 1;
            if(msgsnd(qid, &buff, sizeof(int), 0) < 0)
            {
                perror("msgsnd1");
                exit(1);
            }
        }
        
        buff.mnum = 0;
        msgsnd(qid, &buff, sizeof(int), 0);
        fclose(f);

        return 0;
    }

    pid = fork();
    if(pid == -1)
    {
        perror("Fork failed");
        exit(1);
    }

    if(pid == 0)
    {
        FILE* f = fopen("drugabr.txt", "r");
        if(f == NULL)
        {
            perror("File failure");
            exit(1);
        }

        char br[len];
        while(fgets(br, len, f))
        {
            buff.mtype = 2;
            buff.mnum = atoi(br);
            if(msgsnd(qid, &buff, sizeof(int), 0) < 0)
            {
                perror("msgsnd2");
                exit(1);
            }
        }
        
        buff.mnum = 0;
        msgsnd(qid, &buff, sizeof(int), 0);
        fclose(f);

        return 0;
    }

    int sum1 = 0, num1 = 1;
    int sum2 = 0, num2 = 1;

    while(num1 || num2)
    {
        if(num1)
        {
            msgrcv(qid, &buff, sizeof(int), 1, 0);
            num1 = buff.mnum;
            sum1 += num1;
        }

        if(num2)
        {
            msgrcv(qid, &buff, sizeof(int), 2, 0);
            num2 = buff.mnum;
            sum2 += num2;
        }

        printf("Suma1: %d, Suma2: %d\n", sum1, sum2);
    }

    if(sum1 > sum2)
        printf("Suma prvog procesa je veca\n");
    else if(sum1 < sum2)
        printf("Suma drugog procesa je veca\n");
    else
        printf("Sume su jednake\n");

    wait(NULL);
    msgctl(qid, IPC_RMID, NULL);
    return 0;
}
