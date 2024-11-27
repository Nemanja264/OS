#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int main()
{
  int pid;
  int pd1[2], pd2[2];
  if (pipe(pdl) < 0) exit(1);
  if (pipe(pd2) < 0) exit(1);
  
  pid = fork();
  
  if (pid==-1)
  {
    perror("Fork failed");
    exit(1);
  }
  else if (pid == 0)
  {
  
    close(pd1[1]); close(pd2[0]);
    char ime [24];
    char key [20];
    char line [100];
    
    read (pd1[0], ime, 24);
    read (pd1[0], key, 20);
    
    FILE* f = fopen(ime, "r"); 
    if(f == 0)
      exit(2);
    
    int lineNum = 0;
    while(fgets(line, 100, f)){
      LineNum++;
      
      if(strstr(line, key) != NULL)
        write(pd2[1], &lineNum, sizeof(int));
    }
    fclose(f);
    close(pd1[0]);
    close(pd2[1]);
    wait (NULL);
  }
  else
  {
    close(pd1[0]); close(pd2[1]);
    char path[24];
    char keyword [20];
    printf("Unesi putanju do datoteke: ");
    scanf("%s", path);
    printf("Unesi kljucnu rec: ");
    scanf("%s", keyword);
  
    write(pd1[1], path, sizeof(path));
    write(pd1[1], keyword, sizeof(keyWord));
  
    int num;
    while(read (pd2[0], &num, sizeof(int)))
    {
      printf("Key words found in line: %d\n", num);
    }
    
    close(pd1[1]);
    close(pd2[0]);
    wait (NULL);
  }
  return 0;
}
