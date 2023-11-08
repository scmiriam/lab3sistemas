#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define N_THREADS 8
int usaA = 0, usaB = 0;
pthread_mutex_t mutexB;
pthread_mutex_t mutexA;
pthread_cond_t turnoA,turnoB;

void *h1(void *arg){
  int n= *(int*)arg;
  pthread_mutex_lock(&mutexA);
  while(usaA) {
    pthread_cond_wait(&turnoA, &mutexA);
  }
  usaA++;
  printf("Thread h1 number %d (id.%llu)  accessing file A        (%d %d)\n", n, pthread_self(), usaA, usaB);
  sleep(rand()% 2);
  usaA--;
  printf("Thread h1 number %d (id.%llu)  releases file A         (%d %d)\n", n, pthread_self(), usaA, usaB);
  pthread_cond_signal(&turnoA);
  pthread_mutex_unlock(&mutexA);
  pthread_exit(0);
}
void *h2( void *arg){
  int n= *(int*)arg;
  pthread_mutex_lock(&mutexB);
  while(usaB) {
    pthread_cond_wait(&turnoB, &mutexB);
  }
  usaB++;
  printf("Thread h2 number %d (id.%llu)  accessing file B        (%d %d)\n", n, pthread_self(), usaA, usaB);
  sleep(rand()% 2);
  usaB--;
  printf("Thread h2 number %d (id.%llu)  releases file B         (%d %d)\n", n, pthread_self(), usaA, usaB);
  pthread_cond_signal(&turnoB);
  pthread_mutex_unlock(&mutexB);
  pthread_exit(0);
}
void *h3(void *arg){
  int n= *(int*)arg;
  pthread_mutex_lock(&mutexA);
  pthread_mutex_lock(&mutexB);

  while(usaA && usaB){
    pthread_cond_wait(&turnoA, &mutexA);
    pthread_cond_wait(&turnoB, &mutexB);
  }
  usaA++;
  usaB++;
  printf("Thread h3 number %d (id.%llu)  accessing files A and B  (%d %d)\n",n, pthread_self(), usaA, usaB );
  sleep(rand()% 2);
  usaA--;
  usaB--;
  printf("Thread h3 number %d (id.%llu)  releases files A and B   (%d %d)\n",n, pthread_self(), usaA, usaB );
  pthread_cond_signal(&turnoA);
  pthread_cond_signal(&turnoB);
  pthread_mutex_unlock(&mutexA);
  pthread_mutex_unlock(&mutexB);
  pthread_exit(0);
}

int main() {
    pthread_t hilo1[N_THREADS], hilo2[N_THREADS], hilo3[N_THREADS];
    pthread_mutex_init(&mutexA, NULL);
    pthread_mutex_init(&mutexB, NULL);
    pthread_cond_init(&turnoA, NULL);
    pthread_cond_init(&turnoB, NULL); 
    int i, nhilo[N_THREADS];
    for (i = 0; i < N_THREADS; i++) {
      nhilo[i] = i;
      pthread_create(&hilo1[i], NULL, h1, (void *) &nhilo[i]);
      pthread_create(&hilo2[i], NULL, h2, (void *) &nhilo[i]);
      pthread_create(&hilo3[i], NULL, h3, (void *) &nhilo[i]);
    }
    for(i=0;i<N_THREADS;i++) {
      pthread_join(hilo1[i],NULL);
      pthread_join(hilo2[i],NULL);
      pthread_join(hilo3[i],NULL);
    }
    pthread_cond_destroy(&turnoA);
    pthread_cond_destroy(&turnoB);
    pthread_mutex_destroy(&mutexA);
    pthread_mutex_destroy(&mutexB);
    exit(0);

}