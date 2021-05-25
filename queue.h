#ifndef QUEUE_H
#define QUEUE_H


// estrutura utilizada para guardar dados de leitura
typedef struct {
   int value; // valor medido pelo sensor
   int sensor_id; // id do sensor que realizou a leitura
   int reading_id; // id da leitura realizada 
} sensor_measurement;

struct node{
    sensor_measurement *data;
    struct node *next;
    struct node *prev;
};

struct queue{
    struct node *head;
    struct node *tail;
    struct node *current_node;
    int max_size;
    int current_size;
};

// função para alocar espaço na memória e inicializar a fila.
int queue_init(struct queue *q, int max_value);
// função que retorna o tamanho atual da fila.
//int current_size(struct queue *q);
// função resnponsável por inserir um elemento na fila.
int insert(struct queue *q, sensor_measurement *value);
// função responsável por emitir os alertas de acordo com as condições propostas pelo enunciado do problema.
int temperature_control(struct queue *q, int id);


#endif