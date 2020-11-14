#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include "fifo.h"

//-----Function Prototypes-----//
fifo *read_preorder();
btree *construct_btree(fifo *fifo);
int pathfromNodeToRoot(fifo *fifo, btree_node *node, btree_node *root);
int pathfromRootToNode(fifo *fifo, btree_node *root, btree_node *node);
int pathfromNodeToNode(fifo *fifo, btree *btree, btree_node *node1, btree_node *node2);
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

    if (pathfromNodeToNode(fifo, btree, node1, node2) == 0) {
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
        data = dequeue(fifo);
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
                dequeue(fifo);
            }
            return 1;
        }
    }

    return 0;
}

// *** pathfromNodeToNode *** //
int pathfromNodeToNode(fifo *fifo, btree *btree, btree_node *node1, btree_node *node2) {
    btree_node *curr_root = btree->root;
    
    // If the nodes are in a different subtree we follow the path node1->root->node2,
    // otherwise if they are on the same subtree we take as new root the root of the 
    // subtree. By doing this all cases are reduced to the initial one
    while (curr_root != NULL) {
        if (node1->data < curr_root->data && node2->data < curr_root->data) {
            curr_root = curr_root->left;
        }
        else if (node1->data > curr_root->data && node2->data > curr_root->data) {
            curr_root = curr_root->right;
        }
        else {
            if (pathfromNodeToRoot(fifo, node1, curr_root) != 1) {
                break;
            }

            if (pathfromRootToNode(fifo, curr_root, node2) != 1) {
                break;
            }
            
            return 1;
        }
    }

    return 0;
}

void print_path(fifo *fifo) {
    int data = 0;
    
    printf("\nPath is: ");
    while (isFifoEmpty(fifo) == false) {
        data = dequeue(fifo);
        if (fifo->size != 0) {
            printf("%d ", data);
        }
        else {
            printf("%d\n", data);
        }
    }
}