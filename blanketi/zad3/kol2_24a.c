#include "mtxt.h"

int main(int argc, char* argv[])
{
    if(argc<2)
    {
        printf("Unesi naziv datoteke kao argument\n");
        exit(1);
    }

    int qid = msgget(KEY, IPC_CREAT|0666);
    struct msgbuf buff;
    int pid;

    pid = fork();
    if(pid == 0)
    {
        FILE* p = fopen("z.txt", "w");
        if(p == NULL)
        {
            perror("File1 failure");
            exit(1);
        }
            

        while(1){
            if(msgrcv(qid, &buff, LEN, 1, 0) < 0)
            {
                perror("Msgrcv1");
                exit(1);
            }

            if(strcmp(buff.mtext, "KRAJ") == 0)
                break;
            
            fprintf(p, "%s", buff.mtext);
        }

        fclose(p);
        return 0;
    }

    pid = fork();
    if(pid == 0)
    {
        FILE* d = fopen("ostali.txt", "w");
        if(d == NULL)
        {
            perror("File1 failure");
            exit(1);
        }

        while(1){

            if(msgrcv(qid, &buff, LEN, 2, 0) < 0)
            {
                perror("Msgrcv2");
                exit(1);
            }

            if(strcmp(buff.mtext, "KRAJ") == 0)
                break;

            fprintf(d, "%s", buff.mtext);
        }

        fclose(d);
        return 0;
    }

    FILE* f = fopen(argv[1], "r");
    if(f == NULL)
    {
        perror("File failure");
        exit(1);
    }

    while(fgets(buff.mtext, LEN, f))
    {
        if(buff.mtext[0] == 'z' || buff.mtext[0] == 'Z')
        {
            buff.mtype = 1;
            if(msgsnd(qid, &buff, LEN, 0) < 0)
            {
                perror("Msgsnd1");
                exit(1);
            }
        }
        else
        {
            buff.mtype = 2;
            if(msgsnd(qid, &buff, LEN, 0) < 0)
            {
                perror("Msgsnd2");
                exit(1);
            }    
        }
    }

    strcpy(buff.mtext, "KRAJ");
    buff.mtype = 1;
    msgsnd(qid, &buff, sizeof(buff.mtext), 0);
    buff.mtype = 2;
    msgsnd(qid, &buff, sizeof(buff.mtext), 0);

    msgctl(qid, IPC_RMID, NULL);
    return 0;
}
