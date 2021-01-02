#include <stdio.h>
#include <stdlib.h>
#include "tree.h"

//------------Enums------------//
typedef enum colours_t {
	red,
	black
} colours;

typedef enum colour_combs_t {
	redred,
	redblack,
	blackred,
	blackblack
} colour_combs;

//-----------Structs-----------//
typedef struct treepattern_t {
	int *array;
	int *used_combinations;
	int size;
	int fullness;
} treepattern;

//-----Function Prototypes-----//
btree *construct_btree();
void clear_used_combinations(treepattern *pattern, int start);
int continue_pattern(btree_node *root, colours colour, treepattern *pattern);
bool isTreeRedBlack(btree *tree, treepattern *pattern);
void make_preorder_array(btree_node *root, int *preorder_array, int *index);
void print_rbtree_preorder(btree tree,  int *preorder_array, treepattern *pattern);
void show_rbtree(btree *tree, int *preorder_array, treepattern *pattern);

//------------Main-------------//
int main(int argc, char *argv[]) {
    btree *tree = construct_btree();
    int *preorder_array;
	treepattern *pattern;

	pattern = (treepattern *) malloc(sizeof(treepattern));
	if (pattern == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}

	pattern->fullness = 0;
	pattern->size = tree->size;
	pattern->array = (int *) malloc((pattern->size)*sizeof(int));
	if (pattern->array == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	
	pattern->used_combinations = (int *) calloc(pattern->size, sizeof(int));
	if (pattern->used_combinations == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}

	if (isTreeRedBlack(tree, pattern) == true) {	
		preorder_array = (int *) malloc((tree->size)*sizeof(int));
		if (preorder_array == NULL) {
			perror("malloc");
			exit(EXIT_FAILURE);
		}

		printf("RB OK\n\n");
		print_rbtree_preorder(*tree, preorder_array, pattern);
		show_rbtree(tree, preorder_array, pattern);
		free(preorder_array);
	}
	else {
		printf("RB NOK\n\n");
	}

	free(pattern->array);
	free(pattern->used_combinations);
	free(pattern);
    delete_btree(tree->root);
    free(tree);

    return 0;
}

//---Funtion Implementations---//

// *** construct_btree *** //
btree *construct_btree() {
    btree *tree = create_btree();
    int data = 0;

    // Read the pre-order traversal of a binary
    // tree and construct that tree 
    scanf("%d",&data);
    while (data >= 0) {
        add_node(tree, data);
        scanf("%d", &data);
    }

    return tree;
}

// *** clear_used_combinations *** //
void clear_used_combinations(treepattern *pattern, int start) {
	int i;
	
	for (i = start; i < pattern->size; i++) {
		pattern->used_combinations[i] = 0;
	}
}

// *** continue_pattern *** //
int continue_pattern(btree_node *root, colours colour, treepattern *pattern) {
	int i, index, left_index, right_index, start;
	int leftBlackDepth = 0, rightBlackDepth = 0;
	colour_combs combination;
	
	if (root == NULL) {
		return 0;
	}

	if (colour == black) {
		// If root does not have any children just
		// paint it black and return 1 for blackdepth
		if (root->left == NULL && root->right == NULL) {
			index = pattern->fullness++;
			pattern->used_combinations[index] = 4;
			pattern->array[index] = black;
			return 1;
		}
		// If root has only one child we can only paint it red
		// in order to have a valid blackdepth. The child can't have any
		// children since it/they should be black, which would produce
		// an invalid blackdepth for both subtree root and its child
		else if (root->right != NULL && root->left == NULL) {
			index = pattern->fullness++;
			pattern->used_combinations[index] = 4;
			if (continue_pattern(root->right, red, pattern) == 0) {	
				pattern->array[index] = black;
				return 1;
			}

			return -1;
		}
		else if (root->left != NULL && root->right == NULL) {
			index = pattern->fullness++;
			pattern->used_combinations[index] = 4;
			if (continue_pattern(root->left, red, pattern) == 0) {	
				pattern->array[index] = black;
				return 1;
			}

			return -1;
		}
		// If root has two children, we try all possible combinations of colours for the children
		// as well as their children (in case one or both are black) until one succeeds or all fail
		else {
			index = pattern->fullness;
			start = pattern->used_combinations[index];
			for (i = start; i < 4; i++) {
				pattern->fullness = index + 1;
				clear_used_combinations(pattern, index + 1);
				combination = i;

				switch (combination) {
					case redred: {
						leftBlackDepth = continue_pattern(root->left, red, pattern);
						if (leftBlackDepth == -1) {
							continue;
						}

						rightBlackDepth = continue_pattern(root->right, red, pattern);
						if (rightBlackDepth == -1) {
							continue;
						}

						if ((leftBlackDepth == rightBlackDepth) && (leftBlackDepth != -1)) {
							pattern->used_combinations[index] = i + 1;
							pattern->array[index] = black;
							return ++leftBlackDepth;
						}
						break;
					}
					case redblack: {
						leftBlackDepth = continue_pattern(root->left, red, pattern);
						if (leftBlackDepth == -1) {
							continue;
						}
						
						right_index = pattern->fullness;
						while (pattern->used_combinations[right_index] != 4) {
							rightBlackDepth = continue_pattern(root->right, black, pattern);
							if (leftBlackDepth == rightBlackDepth) {
								pattern->used_combinations[index] = i + 1;
								pattern->array[index] = black;
								return ++leftBlackDepth;
							}

							pattern->fullness = right_index;
							clear_used_combinations(pattern, right_index + 1);
						}
						break;
					}
					case blackred: {
						left_index = pattern->fullness;
						while (pattern->used_combinations[left_index] != 4) {
							leftBlackDepth = continue_pattern(root->left, black, pattern);
							if (leftBlackDepth == -1) {
								pattern->fullness = left_index;
								clear_used_combinations(pattern, left_index + 1);
								continue;
							}

							rightBlackDepth = continue_pattern(root->right, red, pattern);
							if (rightBlackDepth == -1) {
								break;
							}

							if (rightBlackDepth == leftBlackDepth) {
								pattern->used_combinations[index] = i + 1;
								pattern->array[index] = black;
								return ++leftBlackDepth;
							}

							pattern->fullness = left_index;
							clear_used_combinations(pattern, left_index + 1);
						}
						break;
					}
					case blackblack: {
						left_index = pattern->fullness;
						while (pattern->used_combinations[left_index] != 4) {
							leftBlackDepth = continue_pattern(root->left, black, pattern);
							if (leftBlackDepth == -1) {
								pattern->fullness = left_index;
								clear_used_combinations(pattern, left_index + 1);
								continue;
							}

							right_index = pattern->fullness;
							while (pattern->used_combinations[right_index] != 4) {
								rightBlackDepth = continue_pattern(root->right, black, pattern);
								if (leftBlackDepth == rightBlackDepth) {
									pattern->used_combinations[index] = i + 1;
									pattern->array[index] = black;
									return ++leftBlackDepth;
								}

								pattern->fullness = right_index;
								clear_used_combinations(pattern, right_index + 1);
							}

							pattern->fullness = left_index;
							clear_used_combinations(pattern, left_index + 1);
						}
						break;
					}
					default: {
						fprintf(stderr, "Error in continue_pattern: invalid colour combination given\n");
						exit(EXIT_FAILURE);
					}
				}
			}
			pattern->used_combinations[index] = i;
			return -1;
		}
	}
	else if (colour == red) {
		// If root does not have any children just
		// paint it red and return 0 for blackdepth
		if (root->left == NULL && root->right == NULL) {
			index = pattern->fullness++;
			pattern->array[index] = red;
			return 0;
		}
		// If root has only one child, the blackdepth cannot be the
		// same in both subtrees, therefore root cannot be red
		else if (root->left == NULL || root->right == NULL) {
			return -1;
		}
		// If root has two children, we can only paint them
		// black and try all possible colour combinations for 
		// their children until one succeeds or all fail
		else {
			index = pattern->fullness++;
			left_index = pattern->fullness;
			while (pattern->used_combinations[left_index] != 4) {
				leftBlackDepth = continue_pattern(root->left, black, pattern);
				if (leftBlackDepth == -1) {
					pattern->fullness = left_index;
					clear_used_combinations(pattern, left_index + 1);
					continue;
				}

				right_index = pattern->fullness;
				while (pattern->used_combinations[right_index] != 4) {
					rightBlackDepth = continue_pattern(root->right, black, pattern);
					if (leftBlackDepth == rightBlackDepth) {
						pattern->array[index] = red;
						return leftBlackDepth;
					}

					pattern->fullness = right_index;
					clear_used_combinations(pattern, right_index + 1);
				}

				pattern->fullness = left_index;
				clear_used_combinations(pattern, left_index + 1);
			}
			return -1;
		}
	}
	else {
		fprintf(stderr, "Error in continue_pattern: invalid colour given\n");
		exit(EXIT_FAILURE);
	}
}

// *** isTreeRedBlack *** //
bool isTreeRedBlack(btree *tree, treepattern *pattern) {
	if (tree->root == NULL) {
		fprintf(stderr, "Error in isTreeRedBlack: tree is empty\n");
		return false;
	}

	if (continue_pattern(tree->root, black, pattern) == -1) {
		return false;
	}

	return true;
}

// *** make_preorder_array *** //
void make_preorder_array(btree_node *root, int *preorder_array, int *index) {
    if (root != NULL) {
        preorder_array[(*index)++] = root->data;
        make_preorder_array(root->left, preorder_array, index);
        make_preorder_array(root->right, preorder_array, index);
    }
}

// *** print_rbtree_preorder *** //
void print_rbtree_preorder(btree tree,  int *preorder_array, treepattern *pattern) {
	int i, index = 0;

	make_preorder_array(tree.root, preorder_array, &index);
	for (i = 0; i < tree.size; i++) {
		if (pattern->array[i] == red) {
			printf("%d#R ", preorder_array[i]);
		}
		else if (pattern->array[i] == black) {
			printf("%d#B ", preorder_array[i]);
		}
		else {
			fprintf(stderr, "Error in print_rbtree_preorder: invalid colour found\n");
			exit(EXIT_FAILURE);
		}
	}
	putchar('\n');
}

// *** show_rbtree *** //
// Creates an image of the tree for debugging purposes
void show_rbtree(btree *tree, int *preorder_array, treepattern *pattern) {
	FILE *fptr = NULL;
	char command[35] = {'\0'};
	btree_node *curr;
	int i;

	fptr = fopen("rbtree.dot", "w");
	if (fptr == NULL) {
		fprintf(stderr, "Error in creation of rbtree.dot file\n");
		exit(EXIT_FAILURE);
	}

	fprintf(fptr, "digraph G {\n");
	fprintf(fptr, "\t%d [style=\"filled\",color=\"black\", fontcolor=\"white\"];\n", tree->root->data);
	for (i = 1; i < pattern->size; i++) {
		curr = find_treeNode(tree, preorder_array[i]);
		if (pattern->array[i] == red) {
			fprintf(fptr, "\t%d -> %d;\n", curr->parent->data, curr->data);
			fprintf(fptr, "\t%d [style=\"filled\",color=\"red\", fontcolor=\"black\"];\n", curr->data);
		}
		else if (pattern->array[i] == black) {
			fprintf(fptr, "\t%d -> %d;\n", curr->parent->data, curr->data);
			fprintf(fptr, "\t%d [style=\"filled\",color=\"black\", fontcolor=\"white\"];\n", curr->data);
		}
		else {
			fprintf(stderr, "Error in show_rbtree: invalid colour found\n");
			exit(EXIT_FAILURE);
		}
	}
	fprintf(fptr, "}");
	fclose(fptr);
	sprintf(command, "dot -Tpng rbtree.dot -o rbtree.png");
	system(command);
}