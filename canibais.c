/*
  Universidade de Brasília
  aluno:     Léo Moraes da Silva
  matrícula: 16/0011795
  Estudo dirigido 5 - Programação concorrente

*/
#include "stdio.h"
#include "unistd.h"
#include "stdlib.h"
#include "pthread.h"

pthread_mutex_t threads = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cheio = PTHREAD_COND_INITIALIZER;
pthread_cond_t vazio = PTHREAD_COND_INITIALIZER;


#define tam 13
#define lock   pthread_mutex_lock
#define unlock pthread_mutex_unlock
#define wait   pthread_cond_wait
#define signal pthread_cond_signal
#define broadcast pthread_cond_broadcast

int cont = 0;

int produce_random_number(){
  
  return (rand()%10 + 1);
}

void * cozinheiro(void * arg){
  int id = *((int *) arg); 
  while(1){
    lock(&threads);
    while (cont > 0){
      //condicao de espera do cozinheiro
      wait(&cheio,&threads);
    }    
    cont = produce_random_number();
    printf("\nCozinheiro está produzindo %d refeicoes...\n",cont );
    sleep(2);

    if (cont != 0){
      printf("Cozinheiro avisa que as refeicoes estao prontas\n\n");
      //se a acao do canibal for possivel o chame
      broadcast(&vazio);
    }
    unlock(&threads);  
  }
}

void * canibal(void * arg){
  int id = *((int *) arg); 
  while(1){    
    sleep(1);
    lock(&threads);
    while(cont == 0){
      //enquanto o sinal de n vazio n for dado, espere
      wait(&vazio,&threads);
    }
    printf("Canibal %dº comeu a %dº refeicao\n",id,cont );
    cont--;
    if (cont == 0){
      printf("Canibal %dº avisou que acabaram as refeicoes\n",id );
      //se a acao do cozinheiro for possivel o chame
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

  pthread_t a[0];
  pthread_t b[tam];

  for (i = 0; i < tam ; i++) {
    id = (int *) malloc(sizeof(int));
    *id = i;
    pthread_create(&b[i], NULL, canibal, (void *) (id));    
  }
  pthread_create(&a[0], NULL, cozinheiro, (void *) (id));  

  pthread_join(a[0],NULL);    

  for (i = 0; i < tam ; i++) {
    pthread_join(b[i],NULL);    
  }

  return 0;
}

