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

#define num_clientes 3
#define lock pthread_mutex_lock
#define unlock pthread_mutex_unlock
#define wait pthread_cond_wait
#define signal pthread_cond_signal
#define broadcast pthread_cond_broadcast
#define semaphore sem_t
#define down sem_wait
#define up sem_post
#define try_down sem_trywait

// OBS.: 
/* 0:tabaco (cliente 0 tem tabas)
1: seda (cliente 1 tem seda)
2: fosforo ' ' ' ' ' ' ' ' '  */

semaphore turn;
semaphore tabaco;
semaphore fosforo;
semaphore seda;
pthread_t clientes[num_clientes];
pthread_t vendedor;

int produce_random_number()
{
    return (int) (rand() % 3);
}

void * acao_cliente(void *arg)
{    
    int id = *((int *)arg);
    int t,s,f;
    while (1) {        
        switch (id) {
        case 0:
            // Se a seda e fosforo estiverem disponiveis executa sua ação
            sem_getvalue(&seda,&s);
            sem_getvalue(&fosforo,&f);
            if ( s == 1 && f == 1) {                
                down(&seda);
                down(&fosforo);
                printf("Cliente que tem tabaco age\n");
                up(&turn);
            }
                
            break;
        case 1:
            // Se a tabaco e fosforo estiverem disponiveis executa sua ação
            sem_getvalue(&tabaco, &t);
            sem_getvalue(&fosforo, &f);
            if (t == 1 &&  f == 1)
            {
                down(&tabaco);
                down(&fosforo);
                printf("Cliente que tem seda age\n");
                up(&turn);
            }

            break;
        case 2:
            // Se a seda e tabaco estiverem disponiveis executa sua ação
            sem_getvalue(&tabaco, &t);
            sem_getvalue(&seda, &s);
            if (s == 1 && t == 1)
            {
                down(&seda);
                down(&tabaco);
                printf("Cliente que tem fosforo age\n");
                up(&turn);
            }
            break;            
        default:
            break;
        }
    }
        
}

void up_that_semaphore(int num)
{
    // A partir de um numero, dá um up no semaforo correspondente
    // 0 pra tabaco, 1 pra seda e 2 pra fosforo
    switch (num)
    {
        case 0:
            printf("Vendedor produziu tabas\n");
            up(&tabaco);
            break;
        case 1:
            printf("Vendedor produziu seda\n");
            up(&seda);
            break;
        case 2 :
            printf("Vendedor produziu fosforo\n");
            up(&fosforo);
            break;
        default:
            break;
    }
}

void * acao_vendedor(void *arg)
{
    int produto1 = 0 , produto2 = 0;
    while (1)
    {   
        down(&turn);
        
        produto1 = produce_random_number();
        produto2 = produce_random_number();
        while(produto2 == produto1)
        {
            produto2 = produce_random_number();
        }        
        sleep(1);
        up_that_semaphore(produto1);
        up_that_semaphore(produto2);
    }
}

void main() {    
    time_t t;
    srand((unsigned)time(&t)); 

    sem_init(&turn, 0, 1);
    sem_init(&tabaco, 0, 0);
    sem_init(&fosforo, 0, 0);
    sem_init(&seda, 0, 0);
    
    pthread_create(&vendedor, NULL, acao_vendedor, (void *)(0));
    for (int i = 0; i < num_clientes; i++)
    {
        int *id = (int *)malloc(sizeof(int));
        *id = i;
        pthread_create(&clientes[i], NULL, acao_cliente, (void *)(id));
    }
    for (int i = 0; i < num_clientes; i++)
    {
        pthread_join(clientes[i], NULL);
    }    
}