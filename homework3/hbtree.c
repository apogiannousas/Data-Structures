#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "hbtree.h"

// *** create_hbtree *** //
hbtree *create_hbtree() {
    hbtree *new_btree;

    // Allocate memory for a new binary AVL-tree and initialise it
    new_btree = (hbtree *) malloc(sizeof(hbtree));
    if (new_btree == NULL) {
        exit(EXIT_FAILURE);
    }

    new_btree->root = NULL;
    new_btree->size = 0;

    return new_btree;
}

// *** delete_hbtree *** //
void delete_hbtree(hbtree_node *root) {
    // Check if tree doesn't exist or is empty
    if (root == NULL) {
        return;
    }

    delete_hbtree(root->left);
    delete_hbtree(root->right);
    free(root);
}

// *** find_hbtreeNode *** //
hbtree_node *find_hbtreeNode(hbtree *btree, int data) {
    hbtree_node *curr_node, *parent;

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

// *** find_hbnode *** //
int find_hbnode(hbtree *btree, int data) {
	hbtree_node *node = find_hbtreeNode(btree, data);

	// If node was found return 1, otherwise return 0
    if (node != NULL && node->data == data) {
        return 1;
    }

	return 0;
}

// *** find_rightmost_lefthbnode *** //
hbtree_node *find_rightmost_lefthbnode(hbtree_node *root) {
    hbtree_node *curr_node, *prev_node;

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

// *** find_leftmost_righthbnode *** //
hbtree_node *find_leftmost_righthbnode(hbtree_node *root) {
    hbtree_node *curr_node, *prev_node;

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

// *** find_hbnodeLevel *** //
int find_hbnodeLevel(hbtree *btree, int data) {
    hbtree_node *curr_node;
    int level = 0;

    // Check if tree doesn't exist or is empty and return -1
    if (btree == NULL || btree->root == NULL) {
        return -1;
    }

    curr_node = btree->root;
    while (curr_node != NULL) {
        if (data < curr_node->data) {
            curr_node = curr_node->left;
            level++;
        }
        else if (data > curr_node->data) {
            curr_node = curr_node->right;
            level++;
        }
        else {
            // If node with this value is found return its level
            return level;
        }
    }

    // Return -1 as level if the node does not exist
    return -1;
}

// *** isRightHbChild *** //
bool isRightHbChild(hbtree_node *node) {
    if (node->parent->data < node->data) {
        return true;
    }

    return false;
}

// *** isLeftHbChild *** //
bool isLeftHbChild(hbtree_node *node) {
    if (node->parent->data > node->data) {
        return true;
    }

    return false;
}

// *** balance_factor *** //
int balance_factor(int size) {
	return (max(1, (int) log2(size)));
}

// *** give_height *** //
int give_height(hbtree_node *node) {
	if (node != NULL) {
        return node->height;
	}

	return 0;
}

// *** give_size *** //
int give_size(hbtree_node *node) {
	if (node != NULL) {
		return node->size;
	}

	return 0;
}

// *** update_ancestors *** //
void update_ancestors(hbtree_node *starting_node) {
	hbtree_node *curr;

	// Go from the starting_node to root and update the height and size of every node
	for (curr = starting_node; curr != NULL; curr = curr->parent) {
		curr->height = max(give_height(curr->left), give_height(curr->right)) + 1;
		curr->size = give_size(curr->left) + give_size(curr->right) + 1;
	}
}

// *** simple_left_rotation *** //
void simple_left_rotation(hbtree_node *top, hbtree_node *intermediate, hbtree_node *bottom, bool heightupdate) {
	// Make intermediate's left child the new right child of top
	top->right = intermediate->left;
	if (top->right != NULL) {
		top->right->parent = top;
	}

	// Update top's height and size
	top->height = max(give_height(top->left), give_height(top->right)) + 1;
	top->size = give_size(top->left) + give_size(top->right) + 1;

	// Make top's parent the new parent of intermediate
	intermediate->parent = top->parent;
	if (top->parent != NULL) {
		if(isRightHbChild(top) == true) {
			top->parent->right = intermediate;
		}
		else {
			top->parent->left = intermediate;
		}
	}

	// Make top the new left child of intermediate
	intermediate->left = top;
	top->parent = intermediate;
	// Update intermediate's height and size
	intermediate->height = max(top->height, give_height(bottom)) + 1;
	intermediate->size = top->size + give_size(bottom) + 1;
	
	// Heightupdate is set true by add_hbnode and false by remove_hbnode.
	// When true it calls the funtion update_height to 
	if (heightupdate == true) {
		update_ancestors(intermediate->parent);
	}
}

// *** simple_right_rotation *** //
void simple_right_rotation(hbtree_node *top, hbtree_node *intermediate, hbtree_node *bottom, bool heightupdate) {
	// Make intermediate's right child the new left child of top
	top->left = intermediate->right;
	if (top->left != NULL) {
		top->left->parent = top;
	}

	// Update top's height and size
	top->height = max(give_height(top->left), give_height(top->right)) + 1;
	top->size = give_size(top->left) + give_size(top->right) + 1;

	// Make top's parent the new parent of intermediate
	intermediate->parent = top->parent;
	if (top->parent != NULL) {
		if(isRightHbChild(top) == true) {
			top->parent->right = intermediate;
		}
		else {
			top->parent->left = intermediate;
		}
	}

	// Make top the new right child of intermediate
	intermediate->right = top;
	top->parent = intermediate;
	// Update intermediate's height and size
	intermediate->height = max(top->height, give_height(bottom)) + 1;
	intermediate->size = top->size + give_size(bottom) + 1;

	// Heightupdate is set true by add_hbnode and false by remove_hbnode.
	// When true it calls the funtion update_height to 
	if (heightupdate == true) {
		update_ancestors(intermediate->parent);
	}
}

// *** add_hbnode *** //
int add_hbnode(hbtree *btree, int data) {
    hbtree_node *parent = find_hbtreeNode(btree, data);
    hbtree_node *new_node, *curr;
    int leftchild_height, rightchild_height;

    // Check if this node already exists 
    if (parent != NULL && parent->data == data) {
        return 0;
    }

    // Allocate memory for new node and set properly its fields
    new_node = (hbtree_node *) malloc(sizeof(hbtree_node));
    if (new_node == NULL) {
        exit(EXIT_FAILURE);
    }
    new_node->parent = parent;
    new_node->left = NULL;
    new_node->right = NULL;
    new_node->data = data;
	new_node->height = 1;
    new_node->size = 1;

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
	
	curr = new_node->parent;
	while(curr != NULL) {
		// Update height and size for each node
        leftchild_height = give_height(curr->left);
		rightchild_height = give_height(curr->right);
		curr->height = max(leftchild_height, rightchild_height) + 1;
        curr->size++;

		// Apply balancing if the node is unbalanced
		if ((leftchild_height - rightchild_height) > balance_factor(curr->size)) {
			if (curr->left->right == NULL) {
				simple_right_rotation(curr, curr->left, curr->left->left, true);
			}
			else if (curr->left->left == NULL) {
				// Double right rotation
				simple_left_rotation(curr->left, curr->left->right, curr->left->right->right, true);
				simple_right_rotation(curr, curr->left, curr->left->left, true);
			}
			else {
				if (curr->left->left->height >= curr->left->right->height) {
					simple_right_rotation(curr, curr->left, curr->left->left, true);
				}
				else {
					// Double right rotation
					simple_left_rotation(curr->left, curr->left->right, curr->left->right->right, true);
					simple_right_rotation(curr, curr->left, curr->left->left, true);
				}
			}

			if (curr == btree->root) {
				btree->root = curr->parent;
			}
			break;
		}
		else if ((rightchild_height - leftchild_height) > balance_factor(curr->size)) {
			if (curr->right->left == NULL) {
				simple_left_rotation(curr, curr->right, curr->right->right, true);
			}
			else if (curr->right->right == NULL) {
				// Double left rotation
				simple_right_rotation(curr->right, curr->right->left, curr->right->left->left, true);
				simple_left_rotation(curr, curr->right, curr->right->right, true);
			}
			else {
				if (curr->right->right->height >= curr->right->left->height) {
					simple_left_rotation(curr, curr->right, curr->right->right, true);
				}
				else {
					// Double left rotation
					simple_right_rotation(curr->right, curr->right->left, curr->right->left->left, true);
					simple_left_rotation(curr, curr->right, curr->right->right, true);
				}
			}

			if (curr == btree->root) {
				btree->root = curr->parent;
			}
			break;
		}
		curr = curr->parent;
	}

    return 1;
}

// *** remove_hbnode *** //
int remove_hbnode(hbtree *btree, int data) {
    hbtree_node *wanted_node = find_hbtreeNode(btree, data);
    hbtree_node *substitute_node, *curr;
	int leftchild_height, rightchild_height;
	bool nodeisBalanced = true;

    // Check if node was found
    if (wanted_node == NULL || wanted_node->data != data) {
        return 0;
    }

    // If the node has two children give it the value of its leftmost
    // descendant in the right subtree and try to delete the descedant instead.
    if (wanted_node->left != NULL && wanted_node->right != NULL) {
        substitute_node = find_leftmost_righthbnode(wanted_node);
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
    else if (isLeftHbChild(wanted_node) == true) {
        wanted_node->parent->left = (wanted_node->left != NULL ? wanted_node->left : wanted_node->right);
    }
    else {
        wanted_node->parent->right = (wanted_node->left != NULL ? wanted_node->left : wanted_node->right);
    }
    btree->size--;
    curr = wanted_node->parent;
    free(wanted_node);

	while(curr != NULL) {
        // Update height and size for each node
        leftchild_height = give_height(curr->left);
		rightchild_height = give_height(curr->right);
		curr->height = max(leftchild_height, rightchild_height) + 1;
        curr->size = give_size(curr->left) + give_size(curr->right) + 1;

		// Apply balancing if the node is unbalanced
		if ((leftchild_height - rightchild_height) > balance_factor(curr->size)) {
			nodeisBalanced = false;
			if (curr->left->right == NULL) {
				simple_right_rotation(curr, curr->left, curr->left->left, false);
			}
			else if (curr->left->left == NULL) {
				// Double right rotation
				simple_left_rotation(curr->left, curr->left->right, curr->left->right->right, false);
				simple_right_rotation(curr, curr->left, curr->left->left, false);
			}
			else {
				if (curr->left->left->height >= curr->left->right->height) {
					simple_right_rotation(curr, curr->left, curr->left->left, false);
				}
				else {
					// Double right rotation
					simple_left_rotation(curr->left, curr->left->right, curr->left->right->right, false);
					simple_right_rotation(curr, curr->left, curr->left->left, false);
				}
			}

			if (curr == btree->root) {
				btree->root = curr->parent;
			}
		}
		else if ((rightchild_height - leftchild_height) > balance_factor(curr->size)) {
			nodeisBalanced = false;
			if (curr->right->left == NULL) {
				simple_left_rotation(curr, curr->right, curr->right->right, false);
			}
			else if (curr->right->right == NULL) {
				// Double left rotation
				simple_right_rotation(curr->right, curr->right->left, curr->right->left->left, false);
				simple_left_rotation(curr, curr->right, curr->right->right, false);
			}
			else {
				if (curr->right->right->height >= curr->right->left->height) {
					simple_left_rotation(curr, curr->right, curr->right->right, false);
				}
				else {
					// Double left rotation
					simple_right_rotation(curr->right, curr->right->left, curr->right->left->left, false);
					simple_left_rotation(curr, curr->right, curr->right->right, false);
				}
			}

			if (curr == btree->root) {
				btree->root = curr->parent;
			}
		}
		else {
			nodeisBalanced = true;
		}

		if (nodeisBalanced == true) {
			curr = curr->parent;
		}
	}

    return 1;
}

// *** print_hbtree_preorder *** //
void print_hbtree_preorder(hbtree_node *root) {
	if (root == NULL) {
		return;
	}

	printf("%d ", root->data);
	print_hbtree_preorder(root->left);
	print_hbtree_preorder(root->right);
}