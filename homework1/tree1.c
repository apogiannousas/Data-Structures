#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include "fifo.h"
#include "stack.h"

//-----Function Prototypes-----//
stack *read_postorder();
btree *construct_btree(stack *stack);
void print_levelorder(btree *btree);

//------------Main-------------//
int main(int argc, char *argv[]) {
    stack *stack = read_postorder();
    btree *btree = construct_btree(stack);
    print_levelorder(btree);
    free(stack);
    delete_btree(btree->root);
    free(btree);

    return 0;
}

//---Funtion Implementations---//

// *** read_postorder *** //
stack *read_postorder() {
    stack *stack = create_stack();
    int data = 0;

    // Read the post-order traversal of a binary
    // tree and store it in a stack structure
    scanf("%d",&data);
    while (data >= 0) {
        push(stack, data);
        scanf("%d", &data);
    }

    return stack;
}

// *** construct_btree *** //
btree *construct_btree(stack *stack) {
    btree *btree = create_btree();
    int data = 0;

    // Obtain every element from stack and put it
    // in the right place at the binary tree
    while (isStackEmpty(stack) == false) {
        pop(stack, &data);
        add_node(btree, data);
    }

    return btree;
}

// *** print_levelorder *** //
void print_levelorder(btree *btree) {
    fifo *fifo = create_fifo();
    btree_node *curr_node = NULL;
    int data = 0;

    // Enqueue root
    enqueue(fifo, btree->root->data);
    while (isFifoEmpty(fifo) == false) {
        // Dequeue and print front element then
        // proceed to enqueue its children
        dequeue(fifo, &data);
        printf("%d ", data);
        curr_node = find_treeNode(btree, data);
        
        // Check if node was found
        if (curr_node == NULL || curr_node->data != data) {
            exit(EXIT_FAILURE);
        }

        // Enqueue the left child
        if (curr_node->left != NULL) {
            enqueue(fifo, curr_node->left->data);
        }

        // Enqueue the right child
        if (curr_node->right != NULL) {
            enqueue(fifo, curr_node->right->data);
        }
    }

    putchar('\n');
    free(fifo);
}