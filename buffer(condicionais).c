/*
  Universidade de Brasília
  aluno:     Léo Moraes da Silva
  matrícula: 16/0011795
  Estudo dirigido 4 - Programação concorrente

*/

//OBS.: a implementacao de buffer eh meramente ilustrativa
//Serve apenas para mostrar o bom funcionamento do programa
//e nao reflete um uso real de um buffer
#include "stdio.h"
#include "unistd.h"
#include "stdlib.h"
#include "pthread.h"

pthread_mutex_t threads = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cheio = PTHREAD_COND_INITIALIZER;
pthread_cond_t vazio = PTHREAD_COND_INITIALIZER;


#define tam 8
#define N 5
#define lock   pthread_mutex_lock
#define unlock pthread_mutex_unlock
#define wait   pthread_cond_wait
#define signal pthread_cond_signal

int cont = 0;
int buffer[N];

int produce_random_number(){
  
  return (rand()%40 + 10);
}

void * produtor(void * arg){
  int id = *((int *) arg); 
  while(1){
    sleep(1);
    lock(&threads);
    while (cont == N){
      //enquanto o sinal de desenchimento n for dado, espere
      wait(&cheio,&threads);
    }
    cont++;    
    buffer[cont-1] = produce_random_number();

    printf("%dº Colocando no Buffer o numero %d(tamanho atual: %d)\n",id,buffer[cont-1],cont);
    //printa em amarelo a insercao
    printf("   Resultado: ");
    printf("\033[0;33m");
    for (int i = 0; i < N; ++i){
      printf("%d ",buffer[i]);
    }
    printf("\033[0m");
    printf("\n");

    if (cont == 1){
      //se a acao do consumidor for possivel o chame
      signal(&vazio);
    }
    unlock(&threads);  
  }
}

void * consumidor(void * arg){
  int id = *((int *) arg); 
  while(1){    
    sleep(1);
    lock(&threads);
    while(cont == 0){
      //enquanto o sinal de n vazio n for dado, espere
      wait(&vazio,&threads);
    }
    cont--;    
    printf("%dº Tirando do Buffer o numero %d  (tamanho atual: %d)\n",id,buffer[cont],cont);    
    //Da um inteiro negativo pra sinalizar a falta de info no buffer
    buffer[cont] = -1;

    printf("   Resultado: ");
    for (int i = 0; i < N; ++i){
      //printa em vermelho a remocao
      printf("\033[1;31m");
      printf("%d ",buffer[i]);
      printf("\033[0m");
    }
    printf("\n");
    if (cont == N-1){
      //se a acao do produtor for possivel o chame
      signal(&cheio);
    }
    unlock(&threads);
  }

}

int main() {
  int i;
  int *id;

  time_t t;
  srand((unsigned) time(&t));

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

