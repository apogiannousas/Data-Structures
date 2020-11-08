#include <stdlib.h>
#include <stdbool.h>
#include "stack.h"

// *** create_stack *** //
stack *create_stack() {
    stack *new_stack = NULL;

    // Allocate memory for a new stack and initialise it
    new_stack = (stack *) malloc(sizeof(stack));
    new_stack->top = NULL;
    new_stack->size = 0;

    return new_stack;
}

// *** push *** //
void push(stack *stack, int data) {
    stack_node *element = NULL;

    // Allocate memory for new element and put it at the top
    element = (stack_node *) malloc(sizeof(stack_node));
    element->data = data;
    element->nxt = stack->top;
    stack->top = element;
}

// *** pop *** //
int pop(stack *stack) {
    stack_node *top_element;
    int top_element_data;

    // Check if stack is empty
    if (stack == NULL || stack->size == 0) {
        return -1;
    }

    // Make top's next element the new top element
    // delete old top and return it's value 
    top_element = stack->top;
    top_element_data = top_element->data;
    stack->top = stack->top->nxt;
    top_element->nxt = NULL;
    free(top_element);

    return top_element_data;
}

// *** top *** //
int top(stack *stack) {
    // Check if stack is empty
    if (stack == NULL || stack->size == 0) {
        return -1;
    }

    return stack->top->data;
}