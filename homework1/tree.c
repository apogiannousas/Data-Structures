#include <stdlib.h>
#include "tree.h"

// *** create_btree *** //
btree *create_btree() {
    btree *new_btree;

    // Allocate memory for a new binary tree and initialise it
    new_btree = (btree *) malloc(sizeof(btree));
    if (new_btree == NULL) {
        exit(EXIT_FAILURE);
    }

    new_btree->root = NULL;
    new_btree->size = 0;

    return new_btree;
}

// *** delete_btree *** //
void delete_btree(btree_node *root) {
    // Check if tree doesn't exist or is empty
    if (root == NULL) {
        return;
    }

    delete_btree(root->left);
    delete_btree(root->right);
    free(root);
}

// *** find_treeNode *** //
btree_node *find_treeNode(btree *btree, int data) {
    btree_node *curr_node, *parent;

    // Check if tree doesn't exist or is empty
    if (btree == NULL || btree->root == NULL) {
        return NULL;
    }

    curr_node = btree->root;
    parent = curr_node;
    while (curr_node != NULL) {
        // Store current node as it might be the parent of the new one
        parent = curr_node;
        if (data < curr_node->data) {
            curr_node = curr_node->left;
        }
        else if (data > curr_node->data) {
            curr_node = curr_node->right;
        }
        else {
            // If node with this value is found return it
            return curr_node;
        }
    }

    // The node with this value does not exist so return its parent
    return parent;
}

// *** find_rightmost_leftnode *** //
btree_node *find_rightmost_leftnode(btree_node *root) {
    btree_node *curr_node, *prev_node;

    // Check if tree is empty
    if (root == NULL) {
        return NULL;
    }

    // Search and return the rightmost element in left subtree
    prev_node = root;
    curr_node = prev_node->left;
    while (curr_node != NULL) {
        prev_node = curr_node;
        curr_node = curr_node->right;
    }

    return prev_node;
}

// *** find_leftmost_rightnode *** //
btree_node *find_leftmost_rightnode(btree_node *root) {
    btree_node *curr_node, *prev_node;

    // Check if tree is empty
    if (root == NULL) {
        return NULL;
    }

    // Search and return the leftomost element in right subtree
    prev_node = root;
    curr_node = prev_node->right;
    while (curr_node != NULL) {
        prev_node = curr_node;
        curr_node = curr_node->left;
    }

    return prev_node;
}

// *** isRightChild *** //
bool isRightChild(btree_node *node) {
    if (node->parent->data < node->data) {
        return true;
    }

    return false;
}

// *** isLeftChild *** //
bool isLeftChild(btree_node *node) {
    if (node->parent->data > node->data) {
        return true;
    }

    return false;
}

// *** add_node *** //
int add_node(btree *btree, int data) {
    btree_node *parent = find_treeNode(btree, data);
    btree_node *new_node;

    // Check if this node already exists 
    if (parent != NULL && parent->data == data) {
        return 0;
    }

    // Allocate memory for new node and set properly its fields
    new_node = (btree_node *) malloc(sizeof(btree_node));
    if (new_node == NULL) {
        exit(EXIT_FAILURE);
    }
    
    new_node->parent = parent;
    new_node->left = NULL;
    new_node->right = NULL;
    new_node->data = data;

    // If parent exists make new node its child, otherwise 
    // the tree is empty so new node must become the root
    if (parent != NULL) {
        if (data < parent->data) {
            parent->left = new_node;
        }
        else {
            parent->right = new_node;
        }
    }
    else {
        btree->root = new_node;
    }
    btree->size++;

    return 1;
}

// *** remove_node *** //
int remove_node(btree *btree, int data) {
    btree_node *wanted_node = find_treeNode(btree, data);
    btree_node *substitute_node;

    // Check if node was found
    if (wanted_node == NULL || wanted_node->data != data) {
        return 0;
    }

    // If the node has two children give it the value of its rightmost 
    // descendant in the left subtree and try to delete the descedant instead.
    if (wanted_node->left != NULL && wanted_node->right != NULL) {
        substitute_node = find_rightmost_leftnode(wanted_node);
        wanted_node->data = substitute_node->data;
        wanted_node = substitute_node;
    }
    
	// If the current node that is to be deleted has
	// one child, make its parent the child's parent
	if (wanted_node->left != NULL) {
        wanted_node->left->parent = wanted_node->parent;
    }
    else if (wanted_node->right != NULL) {
        wanted_node->right->parent = wanted_node->parent;
    }

    // Due to the above handling of all cases, the node to delete has one or
    // no children so even if the node is root or a left or a right child,
    // we give its place to its child or make it NULL if there isn't one
    if (wanted_node == btree->root) {
        btree->root = (wanted_node->left != NULL ? wanted_node->left : wanted_node->right);
    }
    else if (isLeftChild(wanted_node) == true) {
        wanted_node->parent->left = (wanted_node->left != NULL ? wanted_node->left : wanted_node->right);
    }
    else {
        wanted_node->parent->right = (wanted_node->left != NULL ? wanted_node->left : wanted_node->right);
    }
    btree->size--;
    free(wanted_node);

    return 1;
}