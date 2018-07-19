/*
  Universidade de Brasília
  aluno:     Léo Moraes da Silva
  matrícula: 16/0011795
*/
#include "stdio.h"
#include "unistd.h"
#include "stdlib.h"
#include "pthread.h"
#include "semaphore.h"

#define num_teams 10
#define num_pilots num_teams * 2
#define track_capacity num_teams/2
#define lock   pthread_mutex_lock
#define unlock pthread_mutex_unlock
#define wait   pthread_cond_wait
#define signal pthread_cond_signal
#define broadcast pthread_cond_broadcast
#define semaphore sem_t
#define down sem_wait
#define up   sem_post

pthread_mutex_t threads = PTHREAD_MUTEX_INITIALIZER;
semaphore track;
semaphore team_semaphore[num_teams];

int get_team(int id){
  return (id % num_teams)+1;
}

void * pilot_action(void * arg){  
  int id = *((int *) arg);
  int this_pilot_team = get_team(id);  
  while(1){
    down(&track);
    down(&team_semaphore[this_pilot_team]);
    printf("Piloto(%d)º da equipe %d começa o treino\n",id,this_pilot_team);
    sleep(1);
    printf("Piloto(%d)º da equipe %d termina o treino\n",id,this_pilot_team);
    up(&track);
    up(&team_semaphore[this_pilot_team]);

  }
}

int main() {
  int i;
  int *id;
  pthread_t a[num_pilots];

  //Initialize tack semaphore
  sem_init(&track,0,track_capacity);
  for (int i = 0; i < num_teams; ++i){
    //Initialize team_semaphore
    sem_init(&team_semaphore[i],0,1);
  }

  for (i = 0; i < num_pilots ; i++) {
    id = (int *) malloc(sizeof(int));
    *id = i;
    pthread_create(&a[i], NULL, pilot_action, (void *) (id));    
  }
  
  for (i = 0; i < num_pilots ; i++) {
    pthread_join(a[i],NULL);    
  }

  return 0;
}

