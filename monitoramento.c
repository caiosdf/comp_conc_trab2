#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include "rw.h"
#include "queue.h"

#define MAX 60

struct queue *measurements_list;

void *sensor (void *arg){
    long int id = (long int) arg;
    printf("sensor %ld entrou!\n", id);
    sensor_measurement *measurement;
    measurement = (sensor_measurement *)malloc(sizeof(sensor_measurement));
    int reading_id = 0;

    measurement->sensor_id = id;
    while(1){
        reader_in(id);
        measurement->reading_id = reading_id;
        srand(time(NULL));
        measurement->value = (rand()%15) + 25;
        reading_id++;
        reader_out(id);
        if(measurement->value > 30){
            writer_in(id);
            insert(measurements_list, measurement);
            writer_out(id);
        }
        sleep(1);
    }
}

void *actuator (void *arg){
    long int id = (long int) arg;
    printf("atuador %ld entrou!\n", id);
    struct queue *measurements_by_id;
    struct node *ptr;
    ptr = (struct node *) malloc(sizeof(struct node));
    while(1){
        reader_in(id);
        queue_init(measurements_by_id, MAX);
        ptr = measurements_list->current_node;
        if(measurements_list->current_size < MAX){
            while(ptr != measurements_list->head){
                if(ptr->data->sensor_id == id) insert(measurements_by_id, ptr->data);
                ptr = ptr->prev;
            }
        }else {
            while(ptr->prev != measurements_list->current_node){
                if(ptr->data->sensor_id == id) insert(measurements_by_id, ptr->data);
                ptr = ptr->prev;
            }
        }
        temperature_control(measurements_by_id, id);
        free(measurements_by_id);
        reader_out(id);
        sleep(2);
    }
}

int main(int argc, char **argv){
    pthread_t *tid;
    int nsensors;
    
    //leitura e avaliacao dos parametros de entrada
    if(argc<2) {
        printf("Digite: %s <numero de sensores>\n", argv[0]);
        return 1;
    }
    printf("flag1\n");
    nsensors = atoi(argv[1]);
    // alocação e inicialização das estruturas
    printf("flag2\n");
    queue_init(measurements_list, MAX);
    tid = (pthread_t*) malloc(sizeof(pthread_t)*nsensors*2);
    if(tid==NULL) {
        puts("ERRO--malloc"); 
        return 2;
    }
    printf("flag3 %ld\n", sizeof(pthread_t)*nsensors*2);
    for(long int i=0; i<nsensors; i++) {
        if(pthread_create(tid+i, NULL, sensor, (void*) i)){
            puts("ERRO--pthread_create"); 
            return 3;
        }
    } 
    printf("flag4\n");
    for(long int i=0; i<nsensors; i++) {
        if(pthread_create(tid+nsensors+i, NULL, actuator, (void*) i)){
            puts("ERRO--pthread_create"); 
            return 3;
        }
    } 
    printf("flag5\n");
    return 0;
}