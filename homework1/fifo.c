#include <stdlib.h>
#include "fifo.h"

// *** create_fifo *** //
fifo *create_fifo() {
    fifo *new_fifo;

    // Allocate memory for a new fifo and initialise it
    new_fifo = (fifo *) malloc(sizeof(fifo));
    new_fifo->front = NULL;
    new_fifo->rear = NULL;
    new_fifo->size = 0;

    return new_fifo;
}

// *** isFifoEmpty *** //
bool isFifoEmpty(fifo *fifo) {
    if (fifo->size == 0) {
        return true;
    }

    return false;
}

// *** enqueue *** //
int enqueue(fifo *fifo, int data) {
    fifo_node *element = NULL;

    // Check if there hasn't been memory allocated for fifo
    if (fifo == NULL) {
        return 0;
    }

    // Construct the new element of the fifo
    element = (fifo_node *) malloc(sizeof(fifo_node));
    element->data = data;
    element->nxt = NULL;

    // If fifo is not empty attach the new element in the end
    // , otherwise new element will be both front and rear
    if (isFifoEmpty(fifo) == false) {
        fifo->rear->nxt = element;
        fifo->rear = element;
    }
    else {
        fifo->front = fifo->rear = element;
    }
    fifo->size++;

    return 1;
}

// *** dequeue *** //
int dequeue(fifo *fifo, int *front_element_data) {
    fifo_node *front_element = NULL;
    
    // Check if there hasn't been memory allocated for fifo or if it is empty
    if (fifo == NULL || isFifoEmpty(fifo) == true) {
        return 0;
    }

    // Make front's next element the front, store its data and delete it
    front_element = fifo->front;
    fifo->front = fifo->front->nxt;
    front_element->nxt = NULL;
    *front_element_data = front_element->data;
    fifo->size--;
    free(front_element);

    return 1;
}