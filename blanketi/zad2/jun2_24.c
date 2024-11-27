#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define n 100

int main(int argc, char* argv[])
{
  int pidl, pid2;
  int pd1[2], pd2[2];
  if (pipe(pd1) < 0) exit(1);
  if (pipe(pd2) < 0) exit(1);

  pidl = fork();
  if(pidl != 0)
  {
    close(pd1[0]);
    close(pd2[0]);
    close(pd2[1]);
    
    for(int i = 1; i<argc; i++)
      write(pd1[1], argv[i], n);
    
    close(pd1[1]);
    wait (NULL);
  }
  else if (pid2 = fork() != 0)
  {
    close(pd1[1]);
    close(pd2[0]);
    char ime[n];

    read (pd1[0], ime, n);

    int brKey= argc-2;
    char keyWords[brKey][n];
    int i = 0;
    while(read (pd1[0], keyWords[i++], n));

    FILE * f = fopen(ime, "r");
    if(f == 0)
      exit(1);

    char line[n];
    int lineNum = 1;
    while(fgets(line, 100, f))
    {
      for(int j = 0; j<brKey; ++j)
        if(strstr(line, keyWords[j]) != NULL)
          write(pd2[1], &lineNum, sizeof(int));|
      ++LineNum;
    }
    fclose(f);
    close(pd2[1]);
    close(pd1[0]);
    wait(NULL);
  }
  else
  {
    close(pd2[1]);
    close(pd1[0]);
    close(pd1[1]);
    int zbir = 0;
    int br;
    
    while(read (pd2[0], &br, sizeof(int)))
    {
      printf("%d\n", br);
      zbir += br;
    }
  
    printf("Zbir linija pronadjenih kljucnih reci: %d\n", zbir); 
    close(pd2[0]);
  }
  return 0;
}
