#ifndef _FIFO_H_
#define _FIFO_H_
#include <stdbool.h>

//-----------Structs-----------//
typedef struct fifo_node_t {
    struct fifo_node_t *nxt;
    int data;
} fifo_node;

typedef struct fifo_t {
    //struct stack_node_t *bottom;
    struct fifo_node_t *front;
    struct fifo_node_t *rear;
    int size;
} fifo;

//-----Function Prototypes-----//
fifo *create_fifo();
bool isFifoEmpty(fifo *fifo);
int enqueue(fifo *fifo, int data);
int dequeue(fifo *fifo, int *front_element_data);

#endif