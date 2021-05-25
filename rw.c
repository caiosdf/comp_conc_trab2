#include <pthread.h>
#include <stdio.h>
#include "rw.h"

// variaveis do problema
int readers = 0; // contador de threads lendo
int writers = 0; // contador de threads escrevendo
int writers_waiting = 0; // contador de threads esperando para escrever

// variaveis para sincronização
pthread_mutex_t mutex;
pthread_cond_t cond_readers, cond_writers;

// entrada leitura
void reader_in (int id) {
   pthread_mutex_lock(&mutex);
   printf("L[%d] quer ler\n", id);
   while(writers > 0 || writers_waiting > 0) {
     printf("L[%d] bloqueou\n", id);
     pthread_cond_wait(&cond_readers, &mutex);
     printf("L[%d] desbloqueou\n", id);
   }
   readers++;
   pthread_mutex_unlock(&mutex);
}

// saida leitura
void reader_out (int id) {
   pthread_mutex_lock(&mutex);
   printf("L[%d] terminou de ler\n", id);
   readers--;
   if(readers==0 && writers_waiting > 0) pthread_cond_signal(&cond_writers);
   pthread_mutex_unlock(&mutex);
}

// entrada escrita
void writer_in (int id) {
   pthread_mutex_lock(&mutex);
   printf("E[%d] quer escrever\n", id);
   writers_waiting++;
   while((readers > 0) || (writers > 0)) {
     printf("E[%d] bloqueou\n", id);
     pthread_cond_wait(&cond_writers, &mutex);
     printf("E[%d] desbloqueou\n", id);
   }
   writers_waiting--;
   writers++;
   pthread_mutex_unlock(&mutex);
}

// saida escrita
void writer_out (int id) {
   pthread_mutex_lock(&mutex);
   printf("E[%d] terminou de escrever\n", id);
   writers--;
   if(writers_waiting > 0) pthread_cond_signal(&cond_writers);
   else pthread_cond_broadcast(&cond_readers);
   pthread_mutex_unlock(&mutex);
}


