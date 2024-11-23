#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/types.h>

sem_t start, input;

void* countdown(void* args)
{
  int* n = (int*) args;
  while(1)
  {
    sem_wait(&start);
    if(*n == 999)
      break;
      
    for(int i = 0; i <= *n; i++){
      printf("%d\n", i);
      sleep (2);
      }
    
    sem_post(&input);
  }
  return NULL;
}

int main()
{
  int broj:
  sem_init(&start, 0, 0);
  sem_init(&input, 0, 1);
  pthread t tl;
  pthread_create(&t1, NULL, countdown, (void*)&broj);
  
  do
  {
    sem_wait(&input);
    printf("Unesi broj:"); 
    scanf("%d", &broj); 
    sem_post(&start);
    
  } while (broj != 999);
  
  pthread_join(t1, NULL);
  sem_destroy(&input);
  sem_destroy(&start); 
  return 0;
}
