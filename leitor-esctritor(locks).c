/*
  Universidade de Brasília
  aluno:     Léo Moraes da Silva
  matrícula: 16/0011795
  Estudo dirigido 2 - Programação concorrente

*/
#include "stdio.h"
#include "unistd.h"
#include "stdlib.h"
#include "pthread.h"

pthread_mutex_t bol = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t l   = PTHREAD_MUTEX_INITIALIZER;

#define tam 3

int nl = 0;

void * escritor( void * arg ){    
  int id = *((int *) arg);
  while(1){
    pthread_mutex_lock(&bol);
    printf("writing thread %dº\n",id);
    //tempo de escrita
    //sleep(2);
    pthread_mutex_unlock(&bol);
    printf("finnish writing thread %dº\n",id);
    //tempo de flush
    sleep(5);
  }
  pthread_exit(0);
}

void * leitor( void * arg ) {  
  int id = *((int *) arg); 

  while(1){
    pthread_mutex_lock(&l);
      nl++;
      if (nl == 1){
        pthread_mutex_lock(&bol);
      }  
    pthread_mutex_unlock(&l); 
    printf("Start reading thread %dº\n",id);
    //tempo de processo de leitura
    sleep(1);  
    printf("Finnish reading thread %dº\n",id);
    pthread_mutex_lock(&l);
      nl--;
      if (nl == 0){
        pthread_mutex_unlock(&bol); 
      }
    pthread_mutex_unlock(&l);
    //tempo de processo dps da leitura
    sleep(2);
  }
  pthread_exit(0);
}

int main() {
  int i;
  int *id;
  
  pthread_t a[tam];
  pthread_t b[tam];
  
  for (i = 0; i < tam ; i++) {
    id = (int *) malloc(sizeof(int));
    *id = i;
    pthread_create(&a[i], NULL, escritor, (void *) (id));
    pthread_create(&b[i], NULL, leitor,   (void *) (id));
  }
  for (i = 0; i < tam ; i++) {
    pthread_join(a[i],NULL);    
  }
  
  for (i = 0; i < tam ; i++) {
    pthread_join(b[i],NULL);    
  }
   
  return 0;
}

