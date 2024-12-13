// GLAVNI
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <sys/types.h>

#define RID 10300
#define LEN 255

struct msgbuf
{
    long mtype;
    char mtext[LEN];
};

int main()
{
    int pid;
    int qid;
    struct msgbuf baff;
    qid = msgget(RID, 0666|IPC_CREAT);
    char tmp[LEN];
    int poruka;

    pid = fork();
    if(pid == -1)
    {
        perror("Fork failed");
        exit(1);
    }

    if(pid == 0)
    {
        if(execl("parni", "parni", "pola1", "1", NULL) < 0)
            perror("Greska execl parni");
        
        exit(1);
    }

    pid = fork();
    if(pid == -1)
    {
        perror("Fork failed");
        exit(1);
    }

    if(pid == 0)
    {
        if(execl("neparni", "neparni", "pola2", "2", NULL) < 0)
            perror("Greska execl neparni");
        
        exit(1);
    }

    if(qid < 0)
    {
        perror("Msg failure");
        exit(1);
    }

    FILE* f = fopen("poruke", "r");
    if(f == NULL)
    {
        perror("File failure");
        exit(1);
    }

    poruka = 0;
    while(fgets(tmp, LEN, f) != NULL)
    {
        strcpy(baff.mtext, tmp);

        if(poruka%2)
            baff.mtype = 1;
        else
            baff.mtype = 2;
        
        if(msgsnd(qid, &baff, LEN, 0) < 0)
        {
            perror("Slanje error");
            exit(1);
        }

        poruka++;
    }

    fclose(f);

    strcpy(baff.mtext, "END");

    baff.mtype = 1;
    msgsnd(qid, &baff, LEN, 0);
    baff.mtype = 2;
    msgsnd(qid, &baff, LEN, 0);

    wait(NULL);
    wait(NULL);
    msgctl(qid, IPC_RMID, NULL);
    return 0;
}

// neparni.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <sys/types.h>

#define RID 10300
#define LEN 255

struct msgbuf
{
    long mtype;
    char mtext[LEN];
};
int main(int argc, char* argv[])
{
    int qid;
    struct msgbuf buff;
    
    int tip = atoi(argv[2]);
    strcpy(buff.mtext,"");
    
    FILE* f = fopen(argv[1], "w");
    if(f == NULL)
    {
        perror("File failed");
        exit(1);
    }

    qid = msgget(RID, 0666|IPC_CREAT);

    while(strcmp(buff.mtext, "END") != 0)
    {
        if(msgrcv(qid, &buff, LEN, tip, 0) < 0)
        {
            perror("Primanje poruke greska");
            break;
        }
        
        if(strcmp(buff.mtext, "END") !=0)
        {
            for(int i = 0; i<strlen(buff.mtext); i++)
                buff.mtext[i] = tolower(buff.mtext[i]);
            fprintf(f, "%s", buff.mtext);
            fflush(f);
        }
    }

    fclose(f);
    return 0;
}

// parni.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <sys/types.h>

#define RID 10300
#define LEN 255

struct msgbuf
{
    long mtype;
    char mtext[LEN];
};
int main(int argc, char* argv[])
{
    int qid;
    struct msgbuf buff;
    
    int tip = atoi(argv[2]);
    strcpy(buff.mtext,"");
    
    FILE* f = fopen(argv[1], "w");
    if(f == NULL)
    {
        perror("File failed");
        exit(1);
    }

    qid = msgget(RID, 0666|IPC_CREAT);

    while(strcmp(buff.mtext, "END") != 0)
    {
        if(msgrcv(qid, &buff, LEN, tip, 0) < 0)
        {
            perror("Primanje poruke greska");
            break;
        }
        
        if(strcmp(buff.mtext, "END") !=0)
        {
            for(int i = 0; i<strlen(buff.mtext); i++)
                buff.mtext[i] = toupper(buff.mtext[i]);
            fprintf(f, "%s", buff.mtext);
            fflush(f);
        }
    }

    fclose(f);
    return 0;
}
