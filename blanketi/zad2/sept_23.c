#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <sys/wait.h>

#define n 200

int main(int argc, char* argv[])
{
    int pid;
    int pd[2];
    srand(time(NULL));

    if(pipe(pd) < 0)
        exit(1);
    
    pid = fork();
    if(pid == -1)
    {
        perror("Fork failed");
        exit(1);
    }
    else if(pid == 0)
    {
        close(pd[1]);
        char* upisniFajl = argv[2];
        
        FILE* p = fopen(upisniFajl, "w");
        if(p == 0)
            exit(1);
        
        char buff[n+1];
        while(read(pd[0], buff, n+1))
        {
            int br = rand()%255 + 1;
            char crypted[strlen(buff)];
            
            for(int i = 0; i<strlen(buff); i++){
                crypted[i] = buff[i] ^ br;
            }
           
            fprintf(p, "%d", br);
            fprintf(p, "%s\n", crypted);
        }
        fclose(p);

        close(pd[0]);
    }
    else
    {
        close(pd[0]);
        char* Fajl = argv[1];
        
        FILE* f = fopen(Fajl, "r");
        if(f == 0)
            exit(1);

        char buff[n+1];
        int bytesRead;
        while((bytesRead = fread(buff, 1, 200, f)) > 0)
        {
            buff[bytesRead] = '\0';
            
            write(pd[1], buff, strlen(buff)+1);

        }
        fclose(f);

        close(pd[1]);
        wait(NULL);
    }
    return 0;
}
