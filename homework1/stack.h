#ifndef _STACK_H_
#define _STACK_H_
#include <stdbool.h>

//-----------Structs-----------//
typedef struct stack_node_t {
    struct stack_node_t *nxt;
    int data;
} stack_node;

typedef struct stack_t {
    struct stack_node_t *top;
    int size;
} stack;

//-----Function Prototypes-----//
bool isStackEmpty(stack *stack);
stack *create_stack();
int push(stack *stack, int data);
int pop(stack *stack, int *top_element_data);
int top(stack *stack, int *top_element_data);

#endif