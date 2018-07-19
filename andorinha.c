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
#include "string.h"

#define tam 8
#define andorinha_capacity 6
#define lock   pthread_mutex_lock
#define unlock pthread_mutex_unlock
#define wait   pthread_cond_wait
#define signal pthread_cond_signal
#define broadcast pthread_cond_broadcast
#define semaphore sem_t
#define down sem_wait
#define up   sem_post
#define try_down sem_trywait

pthread_mutex_t threads = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cheio = PTHREAD_COND_INITIALIZER;
pthread_cond_t vazio = PTHREAD_COND_INITIALIZER;

int bag[andorinha_capacity];
int bag_usage = 0;

int produce_random_number(){  
  return (rand()%40 + 10);
}

void print_bag(){
  printf("\n   Mochila: ");
  for(int i =0; i < andorinha_capacity; i++){
    printf("%d ", bag[i]);
  }
  printf("\n");
}

void * produtor(void * arg){
  while(1){
    lock(&threads);
    sleep(1);    
    if(bag_usage == andorinha_capacity){
        // Avisa cheio e espera sinal de vazio
        printf("Andorinha viajando\n"); 
        sleep(1);
        signal(&cheio);
        wait(&vazio,&threads);
    }
    else{
        // Coloca uma carta na mochila
        int random_number = produce_random_number();
        printf("Coloca uma carta na mochila de id %d ",random_number);
        bag[bag_usage] = random_number;
        bag_usage++;
        print_bag();                 
    }
    unlock(&threads);
  }
}

void * consumidor(void * arg){
  while(1){    
    lock(&threads);
    sleep(1);
    if(bag_usage == 0){
      // Avisa vazio espera cheio
      printf("Andorinha viajando\n"); 
        sleep(1);
      signal(&vazio);
      wait(&cheio,&threads);
    }
    else{
      // Tira uma carta da mochila
      bag_usage--;
      printf("Tira uma carta na mochila de id %d ",bag[bag_usage]);
      bag[bag_usage] = -1;
      print_bag();      
    }
    unlock(&threads);
  }
  
}

int main() {
  int i;
  int *id;

  time_t t;
  srand((unsigned) time(&t));

  memset(bag,-1,andorinha_capacity * sizeof(int));

  pthread_t a[tam];
  pthread_t b[tam];

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