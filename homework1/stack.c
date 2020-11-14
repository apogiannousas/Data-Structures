#include <stdlib.h>
#include "stack.h"

// *** create_stack *** //
stack *create_stack() {
    stack *new_stack;

    // Allocate memory for a new stack and initialise it
    new_stack = (stack *) malloc(sizeof(stack));
    if (new_stack == NULL) {
        exit(EXIT_FAILURE);
    }

    new_stack->top = NULL;
    new_stack->size = 0;

    return new_stack;
}

// *** isStackEmpty *** //
bool isStackEmpty(stack *stack) {
    if (stack->size == 0) {
        return true;
    }

    return false;
}

// *** push *** //
void push(stack *stack, int data) {
    stack_node *element = NULL;

    // Check if there hasn't been memory allocated for stack
    if (stack == NULL) {
        return;
    }

    // Allocate memory for new element and put it at the top
    element = (stack_node *) malloc(sizeof(stack_node));
    if (element == NULL) {
        exit(EXIT_FAILURE);
    }
    
    element->data = data;
    element->nxt = stack->top;
    stack->top = element;
    stack->size++;
}

// *** pop *** //
int pop(stack *stack) {
    stack_node *top_element;
    int top_element_data;

    // Check if there hasn't been memory allocated for stack or if it is empty
    if (stack == NULL || isStackEmpty(stack) == true) {
        return 0;
    }

    // Make top's next element the new top element
    // , delete old top and return it's value 
    top_element = stack->top;
    top_element_data = top_element->data;
    stack->top = stack->top->nxt;
    stack->size--;
    top_element->nxt = NULL;
    free(top_element);

    return top_element_data;
}

// *** top *** //
int top(stack *stack) {
    // Check if there has been memory allocated for stack and that it isn't empty
    if (stack != NULL && isStackEmpty(stack) == false) {
        return stack->top->data;
    }

    return 0;
}