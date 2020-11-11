#include <stdio.h>
#include <stdlib.h>
#include "tree.h"

//-----Function Prototypes-----//
int *read_levelorder(int *size);
int add_child(btree_node *parent, int child_data, char typeofchild);
void add_nodesLevelorder (btree_node *parent, int *array, int size, int parent_index);
bool istreeBST(btree_node *root);

//------------Main-------------//
int main(int argc, char *argv[]) {
    int data = 0, size;
    btree *btree = create_btree();
    int *array = read_levelorder(&size);

    // Give binary tree its final size
    btree->size = size;

    // Retrieve first element
    if (size == 0) {
        return 1;
    }
    data = array[0];

    // Make it the root
    if (add_node(btree, data) == 0) {
        return 1;
    }

    // Add all nodes to the binary tree and delete fifo
    add_nodesLevelorder(btree->root, array, size, 0);
    free(array);

    // Root exists so we have a tree and
    // check if it is a binary search tree
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
int *read_levelorder(int *size) {
    int *array = NULL;
    int data = 0;

    // Read the level-order traversal of a binary
    // tree and store it in an array
    *size = 0;
    scanf("%d",&data);
    while (data >= 0) {
        (*size)++;
        array = realloc(array, (*size)*sizeof(int));
        array[(*size) - 1] = data;
        scanf("%d", &data);
    }

    return array;
}

// *** add_child *** //
int add_child(btree_node *parent, int child_data, char typeofchild) {
    btree_node *child;

    // Check if parent exists
    if (parent == NULL) {
        return 0;
    }

    // Allocate memory for child and set properly its fields
    child = (btree_node *) malloc(sizeof(btree_node));
    child->parent = parent;
    child->left = NULL;
    child->right = NULL;
    child->data = child_data;

    // Make the new node either left or right child of the parent
    if (typeofchild == 'l') {
        parent->left = child;
    }
    else {
        parent->right = child;
    }

    return 1;
}

// *** add_nodesLevelorder *** //
// This function is based on the fact that with a levelorder traversal in an array 
// if a node is in a[i] then its children if they exist are in a[2*i + 1] and a[2*i + 2]
void add_nodesLevelorder (btree_node *parent, int *array, int size, int parent_index) {
    int leftchild_index = 2*parent_index + 1;
    int rightchild_index = 2*parent_index + 2;

    // if we reach an index out of the array range means we are done
    if (leftchild_index >= size) {
        return;
    }
    add_child(parent, array[leftchild_index], 'l');

    if (rightchild_index >= size) {
        return;
    }
    add_child(parent, array[rightchild_index], 'r');

    // Do the same for both children
    add_nodesLevelorder(parent->left, array, size, leftchild_index);
    add_nodesLevelorder(parent->right, array, size, rightchild_index);
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