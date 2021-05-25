#include <stdio.h>
#include <stdlib.h>
#include "queue.h"


int queue_init(struct queue *q, int max_value){
    printf("fui chamada!\n");
    q = (struct queue *)malloc(sizeof(struct queue));
    if(q==NULL) {
        puts("ERRO--malloc"); 
        return 2;
    }
    q->head = NULL;
    q->tail = NULL;
    q->current_node = NULL;
    q->max_size = max_value;
    q->current_size = 0;
    return 0;
}

// int current_size(struct queue *q){
//     printf("chamei current_size\n");

//     if(q->head == NULL){
//         printf("q->head aqui é NULL\n");
//         return 0;
//     }
//     else if(q->head == q->tail){
//         printf("q->head aqui é igual a q->tail\n");
//         return 1;
//     }
//     else {
//         printf("cai no else do current size\n");
//         struct node *ptr;
//         ptr = (struct node *)malloc(sizeof(struct node));
//         if(ptr==NULL) {
//             puts("ERRO--malloc"); 
//             return 2;
//         }
//         ptr = q->head;
//         int size = 0;
//         while(ptr != q->tail){
//             size++;
//             ptr = ptr->next;
//         }
//         return size;
//     }
// }

int insert(struct queue *q, sensor_measurement *value){
    struct node *ptr;
    ptr = (struct node *)malloc(sizeof(struct node));
    if(ptr==NULL) {
        puts("ERRO--malloc"); 
        return 2;
    }
    printf("flag 1 insert\n");
    ptr->data = value;
    ptr->next = NULL;
    ptr->prev = NULL;
    printf("flag 2 insert\n");
    //int size = current_size(q);
    printf("flag 3 insert\n");
    if(q->head == NULL){
        printf("vou começar a inserir\n");
        q->head = ptr;
        q->tail = ptr;
        q->current_node = ptr;
        q->current_size++;
        printf("terminei de inserir\n");
        return 0;
    }
    else if (q->current_size >= q->max_size){
        q->current_node->next->data->value = ptr->data->value;
        q->current_node = q->current_node->next;
        return 0;
    }
    else if(q->current_size == q->max_size - 1){
        q->tail->next = ptr;
        q->tail = ptr;
        q->tail->next = q->head;
        q->current_node = q->tail;
        q->head->prev = q->tail;
        q->current_size++;
        return 0;
    }
    else if(q->head == q->tail){
        q->tail->next = ptr;
        q->tail = ptr;
        q->tail->next = NULL;
        q->tail->prev = q->head;
        q->head->next = q->tail;
        q->current_node = q->tail;
        q->current_size++;
        return 0;
    }
    else {
        ptr->prev = q->tail;
        q->tail->next = ptr;
        q->tail = ptr;
        q->current_node = q->tail;
        q->current_size++;
        return 0;
    }
}

int temperature_control(struct queue *q, int id){
    struct node *ptr;
    ptr = (struct node *) malloc(sizeof(struct node));
    if(ptr==NULL) {
        puts("ERRO--malloc"); 
        return 2;
    }
    ptr = q->head;
    int yellow_alert = 0;
    int red_alert = 0;
    int red_alert_count = 0;
    int conditions_iterations = 15;
    int summation = 0;
    int number_of_measurements = 0;
    while(ptr != NULL || number_of_measurements < q->max_size) {
        if(conditions_iterations && ptr->data->value > 35) {
            yellow_alert++;
            red_alert_count++;
        } 
        if(conditions_iterations == 11 && red_alert_count == 5){
            red_alert = 1;
        }
        summation += ptr->data->value;
        ptr = ptr->next;
        conditions_iterations--;
        number_of_measurements++; 
    }
    if(red_alert){
        printf("Alerta Vermelho no sensor %d!!!\n", id);
        printf("A média de medidas feitas pelo sensor %d: %d\n", id, summation/number_of_measurements);
        return 0;
    }
    else if(yellow_alert >= 5){
        printf("Alerta Amarelo no sensor %d!!!\n", id);
        printf("A média de medidas feitas pelo sensor %d: %d", id, summation/number_of_measurements);
        return 0;
    }
    else {
        printf("Condições normais no sensor %d.", id);
        printf("A média de medidas feitas pelo sensor %d: %d", id, summation/number_of_measurements);
        return 0;
    }
}



































































