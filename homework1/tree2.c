#include <stdio.h>
#include <stdlib.h>
#include "tree.h"

//-----Function Prototypes-----//
btree_node **read_levelorder(int *size);
int add_child(btree_node *parent, btree_node *child, char typeofchild);
void add_nodesLevelorder (btree_node **array, int size);
bool istreeBST(btree_node *root);

//------------Main-------------//
int main(int argc, char *argv[]) {
    int size = 0;
    btree *btree = create_btree();
    btree_node **array = read_levelorder(&size);

    btree->size = size;
    if (btree == NULL || size == 0) {
        return 1;
    }
    btree->root = array[0];

    add_nodesLevelorder(array, size);
    free(array);

    if (istreeBST(btree->root) == true) {
        printf("Binary Search Tree\n");
    }
    else {
        printf("Binary Tree\n");
    }
    
    delete_btree(btree->root);
    free(btree);

    return 0;
}

//---Funtion Implementations---//

// *** read_levelorder *** //
btree_node **read_levelorder(int *size) {
    btree_node **array = NULL, **temp = NULL;
    int data = 0;

    // Read the level-order traversal of a binary
    // tree and store it in an array of nodes
    *size = 0;
    scanf("%d",&data);
    while (data >= 0) {
        // Alloate memory for a new node and initialise it
        temp = (btree_node **) realloc(array, ((*size) + 1)*sizeof(btree_node *));
        if (temp == NULL) {
            exit(EXIT_FAILURE);
        }
        array = temp;

        array[*size] = (btree_node *) malloc(sizeof(btree_node));
        if (array[*size] == NULL) {
            exit(EXIT_FAILURE);
        }

        array[*size]->parent = NULL;
        array[*size]->left = NULL;
        array[*size]->right = NULL;
        array[*size]->data = data;

        (*size)++;
        scanf("%d", &data);
    }

    return array;
}

// *** add_child *** //
int add_child(btree_node *parent, btree_node *child, char typeofchild) {
    // Check if parent and child exist
    if (parent == NULL || child == NULL) {
        return 0;
    }

    // Make the child either left or right child of the parent
    // and make parent the child's actual parent
    if (typeofchild == 'l') {
        parent->left = child;
    }
    else {
        parent->right = child;
    }
    child->parent = parent;

    return 1;
}

// *** add_nodesLevelorder *** //
// This function is based on the fact that with a levelorder traversal in an array 
// if a node is in a[i] then its children if they exist are in a[2*i + 1] and a[2*i + 2]
void add_nodesLevelorder (btree_node **array, int size) {
    int leftchild_index, rightchild_index, i;

    for (i = 0; i < size; i++) {
        leftchild_index = 2*i + 1;
        rightchild_index = 2*i + 2;

        // if we reach an index out of the array range means we are done
        if (leftchild_index >= size) {
            return;
        }
        add_child(array[i], array[leftchild_index], 'l');

        if (rightchild_index >= size) {
            return;
        }
        add_child(array[i], array[rightchild_index], 'r');
    }
}

bool istreeBST(btree_node *root) {
    btree_node *rightmost_leftnode = find_rightmost_leftnode(root);
    btree_node *leftmost_rightnode = find_leftmost_rightnode(root);
    bool isleftsubtreeBST = true, isrightsubtreeBST = true;

    // Rightmost left node should have the biggest value
    // in the left subtree yet smaller than root's value
    if (rightmost_leftnode != NULL && rightmost_leftnode->data > root->data) {
        return false;
    }

    // Leftmost right node should have the smallest value
    // in the right subtree yet bigger than root's value
    if (leftmost_rightnode != NULL && leftmost_rightnode->data < root->data) {
        return false;
    }
    
    // If we haven't reached the end of the binary tree check wether
    // the same conditions are true for left and right subtrees
    if (root->left != NULL) {
        isleftsubtreeBST = istreeBST(root->left);
    }

    if (root->right != NULL) {
        isrightsubtreeBST = istreeBST(root->right);
    }

    return (isleftsubtreeBST && isrightsubtreeBST);
}