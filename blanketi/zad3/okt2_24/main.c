#include "mtxt.h"

int main()
{
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
        FILE* f = fopen("prva.txt", "r");
        if(f == 0)
        {
            perror("File failure");
            exit(1);
        }

        buff.mtype = 1;
        while(fgets(buff.mtext, LEN, f))
        {
            if(msgsnd(qid, &buff, sizeof(buff.mtext), 0) < 0)
            {
                perror("Msgsnd 1");
                exit(1);
            }
        }

        strcpy(buff.mtext, "KRAJ");
        if(msgsnd(qid, &buff, LEN, 0) < 0)
        {
            perror("Msgsnd kraj");
            exit(1);
        }

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
        FILE* f = fopen("druga.txt", "r");
        if(f == 0)
        {
            perror("File failure");
            exit(1);
        }

        buff.mtype = 2;
        while(fgets(buff.mtext, LEN, f))
        {
            if(msgsnd(qid, &buff, sizeof(buff.mtext), 0) < 0)
            {
                perror("Msgsnd 2");
                exit(1);
            }
        }

        strcpy(buff.mtext, "KRAJ");
        if(msgsnd(qid, &buff, sizeof(buff.mtext), 0) < 0)
        {
            perror("Msgsnd kraj");
            exit(1);
        }

        fclose(f);
        return 0;
    }
        
    FILE* p = fopen("prva_mod.txt", "w");
    if(p == NULL)
        exit(2);

    FILE* d = fopen("druga_mod.txt", "w");
    if(d == NULL)
        exit(2);

    int done = 0;
    while(done != 2)
    {
        if(msgrcv(qid, &buff, sizeof(buff.mtext), 0, 0) < 0)
        {
            perror("Msgrcv");
            exit(1);
        }

        if(strcmp(buff.mtext, "KRAJ") == 0)
        {
            done++;
            continue;
        }

        if(buff.mtype == 1)
        {
            for(int i = 0; i<strlen(buff.mtext); i++)
                buff.mtext[i] = toupper(buff.mtext[i]);
                    
            fprintf(p, "%s", buff.mtext);
        }
        else if(buff.mtype == 2)
        {
            for(int i = 0; i<strlen(buff.mtext); i++)
                buff.mtext[i] = tolower(buff.mtext[i]);
                    
            fprintf(d, "%s", buff.mtext);
        }
    }

    fclose(p);
    fclose(d);
    wait(NULL);
    wait(NULL);
    msgctl(qid, IPC_RMID, NULL);

    return 0;
}
