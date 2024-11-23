
sem_t upis, sum;
#define N 10

int buff[N];
int uzbir = 0;

void writeToBuff (void* args)
{
  while(uzbir < 1000)
  {
    sem_wait(&upis);
    for(int i=0; i<N; i++)
      buff[i] = rand() % 199 + 1;
    sem_post(&sum);
  }
}
int main()
{
  srand(time(NULL));
  sem_init(&upis, 0, 1); sem_init(&sum, 0, 0);
  
  pthread_t t;
  pthread_create(&t, NULL, writeToBuff, NULL);
  
  while(uzbir < 1000)
  {
    int tzbir = 0;
    sem_wait(&sum);
    for(int i = 0; i<N; i++)
    {
      printf("%d", buff[i]);
      uzbir += buff[i];
      tzbir += buff[i];
    }
    printf("\n");
    
    if(tzbir > 800)
      printf("Zbir zadnjih 10 brojeva je veci od 800\n");
    
    printf("Ukupni zbir: %d\n\n", uzbir);
    sem_post(&upis);
  }
  printf("Ukupni zbir presao 1000\n");
  
  pthread_join(t, NULL);
  
  sem_destroy(&upis);
  sem_destroy(&sum);
  return 0;
}
