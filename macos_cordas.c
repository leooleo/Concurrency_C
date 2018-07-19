/*
  Universidade de Brasília
  aluno:     Léo Moraes da Silva
  matrícula: 16/0011795
  Estudo dirigido 3 - Programação concorrente

*/
#include "stdio.h"
#include "unistd.h"
#include "stdlib.h"
#include "pthread.h"

pthread_mutex_t corda  = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t turno  = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t contA  = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t contB  = PTHREAD_MUTEX_INITIALIZER;


#define tam 10
#define lock   pthread_mutex_lock
#define unlock pthread_mutex_unlock

int firstA = 0;
int firstB = 0;

void * paracima(void * arg){
  int id = *((int *) arg); 
  while(1){
    lock(&turno);
    //printf("macaco A %dº pede pra usar a corda\n",id);
      lock(&contA);
        firstA++;
      unlock(&contA);
      if (firstA == 1){
        lock(&corda);
      }  
    unlock(&turno);
    printf("macaco A %dº esta atravessando corda\n",id);
    sleep(1);
    lock(&contA);
      firstA--;
      printf("macaco A %dº att corda\n",id);
    unlock(&contA);
    
    if (firstA == 0){
      unlock(&corda);
    }
  }
  
}

void * parabaixo(void * arg){
  int id = *((int *) arg); 
  while(1){

    lock(&turno);
      //printf("macaco B %dº pede pra usar a corda\n",id);
      lock(&contB);
        firstB++;
      unlock(&contB);
        if (firstB == 1){
          lock(&corda);
        }    
    unlock(&turno);
    printf("macaco B %dº esta atravessando corda\n",id);
    sleep(1);
    lock(&contB);
      firstB--;
      printf("macaco B %dº att corda\n",id);
    unlock(&contB);
    //sleep(1);
    if (firstB == 0){
      unlock(&corda);
    }
  }  
}

int main() {
  int i;
  int *id;

  pthread_t a[tam];
  pthread_t b[tam];

  for (i = 0; i < tam ; i++) {
    id = (int *) malloc(sizeof(int));
    *id = i;
    pthread_create(&a[i], NULL, paracima,    (void *) (id));
    pthread_create(&b[i], NULL, parabaixo,   (void *) (id));
  }
  for (i = 0; i < tam ; i++) {
    pthread_join(a[i],NULL);    
  }

  for (i = 0; i < tam ; i++) {
    pthread_join(b[i],NULL);    
  }

  return 0;
}

