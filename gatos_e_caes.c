/*
  Universidade de Brasília
  aluno:     Léo Moraes da Silva
  matrícula: 16/0011795
  Programação concorrente
  estudo_dirigido_8.c
*/

#include "stdio.h"
#include "unistd.h"
#include "stdlib.h"
#include "pthread.h"
#include "semaphore.h"

#define num_cachorros 4
#define lock pthread_mutex_lock
#define unlock pthread_mutex_unlock
#define wait pthread_cond_wait
#define signal pthread_cond_signal
#define broadcast pthread_cond_broadcast
#define semaphore sem_t
#define down sem_wait
#define up sem_post
#define try_down sem_trywait

pthread_mutex_t comida = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t vontade_gato = PTHREAD_COND_INITIALIZER;

void *acao_cliente(void *arg) {
    int id = *((int *)arg);
    while (1) {
        lock(&comida);
        wait(&vontade_gato, &comida);
        printf("Cao %d comendo\n", id);
        sleep(1);
        printf("Cao %d termina de comer\n", id);
        unlock(&comida);
        
    }    
    
}

void *acao_gato(void *arg) {
    while (1) {        
        lock(&comida);
        printf("Gato comendo\n");
        sleep(1);
        broadcast(&vontade_gato);
        printf("Gato avisa que acabou\n");
        unlock(&comida);
        sleep(1);
    }    
}

void main() {
    pthread_t cachorros[num_cachorros];
    pthread_t gato;
    pthread_create(&gato, NULL, acao_gato, (void *)(0));
    for (int i = 0; i < num_cachorros; i++)
    {
        int *id = (int *)malloc(sizeof(int));
        *id = i;
        pthread_create(&cachorros[i], NULL, acao_cliente, (void *)(id));
    }
    for (int i = 0; i < num_cachorros; i++)
    {
        pthread_join(cachorros[i], NULL);
    }
}