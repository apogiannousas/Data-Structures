#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include "fifo.h"

//-----Function Prototypes-----//
fifo *read_preorder();
btree *construct_btree(fifo *fifo);
int pathfromNodeToRoot(fifo *fifo, btree_node *node, btree_node *root);
int pathfromRootToNode(fifo *fifo, btree_node *root, btree_node *node);
int pathfromNodeToNode(fifo *fifo, btree_node *root, btree_node *node1, btree_node *node2);
void print_path(fifo *fifo);

//------------Main-------------//
int main(int argc, char *argv[]) {
    fifo *fifo = read_preorder();
    btree *btree = construct_btree(fifo);
    btree_node *node1, *node2;
    int node1_data, node2_data;

    // Re-initialise the fifo to use it again
    fifo->front = NULL;
    fifo->rear = NULL;
    fifo->size = 0;

    // Request two integers
    printf("Enter 2 integers: ");
    scanf("%d %d", &node1_data, &node2_data);

    // Find the nodes with these values in the binary tree
    node1 = find_treeNode(btree, node1_data);
    if (node1 == NULL || node1->data != node1_data) {
        printf("%d not found!\n", node1_data);
        return 1;
    }
    node2 = find_treeNode(btree, node2_data);
    if (node2 == NULL || node2->data != node2_data) {
        printf("%d not found!\n", node2_data);
        return 1;
    }

    // Find the minimum path from node1 to node2
    if (pathfromNodeToNode(fifo, btree->root, node1, node2) == 0) {
        return 1;
    }

    print_path(fifo);
    free(fifo);

    delete_btree(btree->root);
    free(btree);

    return 0;
}

//---Funtion Implementations---//

// *** read_preorder *** //
fifo *read_preorder() {
    fifo *fifo = create_fifo();
    int data = 0;

    // Read the pre-order traversal of a binary
    // tree and store it in a fifo structure
    scanf("%d",&data);
    while (data >= 0) {
        enqueue(fifo, data);
        scanf("%d", &data);
    }

    return fifo;
}

// *** construct_btree *** //
btree *construct_btree(fifo *fifo) {
    btree *btree = create_btree();
    int data = 0;

    // Obtain every element from fifo and put
    // it in the right place at the binary tree
    while (isFifoEmpty(fifo) == false) {
        dequeue(fifo, &data);
        add_node(btree, data);
    }

    return btree;
}

// *** pathfromNodeToRoot *** //
int pathfromNodeToRoot(fifo *fifo, btree_node *node, btree_node *root) {
    btree_node *curr;

    // Go upwards from the node given till 
    // you find root and store the path.
    // Note that we won't add root to the path
    // since we plan to use this function in
    // combination with pathfromRootToNode
    for (curr = node; curr != NULL; curr = curr->parent) {
        if (curr == root) {
            return 1;
        }
        enqueue(fifo, curr->data);
    }

    return 0;
}

// *** pathfromRootToNode *** //
int pathfromRootToNode(fifo *fifo, btree_node *root, btree_node *node) {
    btree_node *curr = root;
    int data = 0;

    // Traverse the tree from root till you 
    // reach the node and store the path
    while (curr != NULL) {
        enqueue(fifo, curr->data);
        if (node->data < curr->data) {
            curr = curr->left;
        }
        else if (node->data > curr->data) {
            curr = curr->right;
        }
        else {
            // If initial node is the root return an empty path
            if (fifo->size == 1) {
                dequeue(fifo, &data);
            }
            return 1;
        }
    }

    return 0;
}

// *** pathfromNodeToNode *** //
int pathfromNodeToNode(fifo *fifo, btree_node *root, btree_node *node1, btree_node *node2) {
    // If the nodes are in a different subtree we follow the path 
    // node1->root->node2, otherwise if they are on the same subtree 
    // we recall the function with the root of the subtree as root
    if (node1->data < root->data && node2->data < root->data) {
        return pathfromNodeToNode(fifo, root->left, node1, node2);
    }
    else if (node1->data > root->data && node2->data > root->data) {
        return pathfromNodeToNode(fifo, root->right, node1, node2);
    }
    else {
        if (pathfromNodeToRoot(fifo, node1, root) != 1) {
            return 0;
        }

        if (pathfromRootToNode(fifo, root, node2) != 1) {
            return 0;
        }
        
        return 1;
    }
}

void print_path(fifo *fifo) {
    int data = 0;
    
    printf("\nPath is: ");
    while (isFifoEmpty(fifo) == false) {
        dequeue(fifo, &data);
        if (fifo->size != 0) {
            printf("%d ", data);
        }
        else {
            printf("%d\n", data);
        }
    }
}