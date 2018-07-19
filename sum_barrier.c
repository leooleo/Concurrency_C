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

#define lock   pthread_mutex_lock
#define unlock pthread_mutex_unlock
#define wait   pthread_cond_wait
#define signal pthread_cond_signal
#define broadcast pthread_cond_broadcast
#define semaphore sem_t
#define down sem_wait
#define up   sem_post
#define try_down sem_trywait

#define matrix_size 10

int matrix[matrix_size][matrix_size];
int res_vet[matrix_size];

pthread_barrier_t barreira;

int soma(int linha){
  int res = 0;
  
  for(int j =0; j <matrix_size; j++){
    res += matrix[linha][j];
  }
  res_vet[linha] = res;

}

void * soma_matriz(void * arg){
  int id = *((int *) arg);  
  int res = 0;
  soma(id);
  pthread_barrier_wait(&barreira);
  if(id == 0){
    for(int i=0; i < matrix_size; i++){
      res += res_vet[i];
    }
    printf("Result: %d\n",res);
  }
}

int main() {
  int i,j;
  int *id;
  
  pthread_t a[matrix_size];  
  pthread_barrier_init(&barreira, NULL, matrix_size);

  for (i = 0; i < matrix_size ; i++) {
    for (j = 0; j < matrix_size ; j++) {
      matrix[i][j] = i;
    }
  }
  // for (i = 0; i < matrix_size ; i++) {
  //   for (j = 0; j < matrix_size ; j++) {
  //     printf("%d ",  matrix[i][j] );
  //   }
  //   printf("\n");
  // }

  for (i = 0; i < matrix_size ; i++) {
    id = (int *) malloc(sizeof(int));
    *id = i;
    pthread_create(&a[i], NULL, soma_matriz,    (void *) (id));    
  }
  for (i = 0; i < matrix_size ; i++) {
    pthread_join(a[i],NULL);    
  }

  return 0;
}