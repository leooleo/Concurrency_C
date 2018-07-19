/*
  Universidade de Brasília
  aluno:     Léo Moraes da Silva
  matrícula: 16/0011795
  Estudo dirigido 6 - Programação concorrente

*/

// OBSERVAÇÕES IMPORTANTES!

// OBS.: a implementacao de buffer é meramente ilustrativa
// Serve apenas para mostrar o bom funcionamento do programa
// e nao reflete um uso real de um buffer.
// Além disso, usa-se o '-1' para representar a consumação do dado.

#include "stdio.h"
#include "unistd.h"
#include "stdlib.h"
#include "pthread.h"
#include "semaphore.h"

#define tam 8
#define buffer_size 6
#define lock   pthread_mutex_lock
#define unlock pthread_mutex_unlock
#define wait   pthread_cond_wait
#define signal pthread_cond_signal
#define broadcast pthread_cond_broadcast
#define semaphore sem_t
#define down sem_wait
#define up   sem_post
#define try_down sem_trywait

int cont = 0;
int buffer[buffer_size];
semaphore semaphore_turn;
// Available space in buffer
semaphore buffer_free_space;
// current size of buffer
semaphore buffer_curr_space;


int produce_random_number(){  
  return (rand()%40 + 10);
}

void * produtor(void * arg){
  // buffer_free_space is the semaphore 'down'ed' by consumer
  int id = *((int *) arg); 
  while(1){
    sleep(2);

    down(&buffer_free_space);
    down(&semaphore_turn);  
    up(&buffer_curr_space);
    // Start consuming data
    cont++;    
    buffer[cont-1] = produce_random_number();

    printf("%dº Colocando no Buffer o numero %d(tamanho atual: %d)\n",id,buffer[cont-1],cont);
    // Print yellow the insertion
    printf("   Resultado: ");
    printf("\033[0;33m");
    for (int i = 0; i < buffer_size; ++i){
      printf("%d ",buffer[i]);
    }
    printf("\033[0m");
    printf("\n");
    // Finnish consuming data

    up(&semaphore_turn);
  }
}

void * consumidor(void * arg){
  // buffer_curr_space is the semaphore 'down'ed' by consumer
  int id = *((int *) arg); 
  while(1){    
    sleep(2);

    down(&buffer_curr_space);    
    down(&semaphore_turn);    
    up(&buffer_free_space);
    
    // Start consuming data
    cont--;    
    printf("%dº Tirando do Buffer o numero %d  (tamanho atual: %d)\n",id,buffer[cont],cont);    
    // -1 represents data consumed
    buffer[cont] = -1;

    printf("   Resultado: ");
    for (int i = 0; i < buffer_size; ++i){
      // Print in red the removal
      printf("\033[1;31m");
      printf("%d ",buffer[i]);
      printf("\033[0m");
    }
    printf("\n");
    // Finnish consuming data
    
    
    up(&semaphore_turn);
  }

}

int main() {
  int i;
  int *id;

  time_t t;
  srand((unsigned) time(&t));

  pthread_t a[tam];
  pthread_t b[tam];

  // Initialize semaphores
  sem_init(&buffer_curr_space,0,0);
  sem_init(&buffer_free_space,0,buffer_size);
  sem_init(&semaphore_turn,0,1);

  for(i = 0; i < buffer_size ; i++) {
    buffer[i] = -1;
  }

  for (i = 0; i < tam ; i++) {
    id = (int *) malloc(sizeof(int));
    *id = i;
    pthread_create(&b[i], NULL, produtor,      (void *) (id));
    pthread_create(&a[i], NULL, consumidor,    (void *) (id));
  }
  for (i = 0; i < tam ; i++) {
    pthread_join(a[i],NULL);
  }

  for (i = 0; i < tam ; i++) {
    pthread_join(b[i],NULL);    
  }

  return 0;
}