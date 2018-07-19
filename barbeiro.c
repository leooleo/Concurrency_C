/*
  Universidade de Brasília
  aluno:     Léo Moraes da Silva
  matrícula: 16/0011795
  Estudo dirigido 8 - "Barbeiro e clientes"
  Programação concorrente
  estudo_dirigido_8.c
*/

#include "stdio.h"
#include "unistd.h"
#include "stdlib.h"
#include "pthread.h"
#include "semaphore.h"

#define num_clientes 10
#define num_cadeiras 2
#define lock         pthread_mutex_lock
#define unlock       pthread_mutex_unlock
#define wait         pthread_cond_wait
#define signal       pthread_cond_signal
#define broadcast    pthread_cond_broadcast
#define semaphore    sem_t
#define down         sem_wait
#define up           sem_post
#define try_down     sem_trywait

semaphore cadeiras;
semaphore barbeiro_semaphore;
semaphore cliente_cadeira;
semaphore cabelo_cortado;
pthread_t clientes[num_clientes];
pthread_t barbeiro;

void * acao_cliente(void * arg){  
  int id = *((int *) arg); 
  while(1){      
    if(try_down(&cadeiras) == 0){
        printf("Cliente %d senta\n", id);
        down(&barbeiro_semaphore);                
        up(&cadeiras);
        up(&cliente_cadeira);
        printf("Cliente %d vai cortar o cabelo(desocupa espera)\n", id);
        down(&cabelo_cortado);        
        up(&barbeiro_semaphore);
        printf("Cliente %d cortou o cabelo\n", id);
        sleep(8);
    }
    else{
        printf("Cliente %d não achou cadeiras, vai embora\n", id);
        sleep(2);
    }
  }
}

void * acao_barbeiro(void * arg){  
    while(1){
        down(&cliente_cadeira);
                
        up(&cabelo_cortado);
    }
}

void main(){    
    sem_init(&cadeiras,0,num_cadeiras);
    sem_init(&barbeiro_semaphore,0,1);
    sem_init(&cliente_cadeira,0,0);
    sem_init(&cabelo_cortado,0,0);

    pthread_create(&barbeiro, NULL, acao_barbeiro,(void *) (0));
    for(int i=0; i < num_clientes; i++){
        int *id = (int *) malloc(sizeof(int));
        *id = i;
        pthread_create(&clientes[i], NULL, acao_cliente,(void *) (id));
    }
    for(int i=0; i < num_clientes; i++){        
        pthread_join(clientes[i],NULL);
    }
}