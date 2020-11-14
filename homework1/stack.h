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
stack *create_stack();
bool isStackEmpty(stack *stack);
void push(stack *stack, int data);
int pop(stack *stack);
int top(stack *stack);

#endif