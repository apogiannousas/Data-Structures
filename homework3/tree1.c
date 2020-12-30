#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dlist.h"
#include "hbtree.h"

//-----Function Prototypes-----//
void init_input_str(char *input_str);
void write_dotfile(FILE *fptr, hbtree_node *root);
void show_hbtree(hbtree *tree, int photonum);
//------------Main-------------//
int main(int argc, char *argv[]) {
	enum command {insert, delete, find};
	enum command command;
    hbtree *tree = create_hbtree();
	char input_str[11] = {'\0'};
    int data, result, i = 0;

	result = scanf("%s", input_str);
	while (result == 1) {
		if (input_str[0] == '-') {
			switch (input_str[1]) {
				case 'i': {
					command = insert;
					break;
				}
				case 'd': {
					command = delete;
					break;
				}
				case 'f': {
					command = find;
					break;
				}
				case 'p': {
					putchar('\n');
					print_hbtree_preorder(tree->root);
					putchar('\n');
					break;
				}
				case 'q': {
					delete_hbtree(tree->root);
					free(tree);
					return 0;
				}
				default: {
					fprintf(stderr, "Error: Wrong flag -%c was given", input_str[1]);
					exit(EXIT_FAILURE);
				}
			}

			init_input_str(input_str);
			result = scanf("%s", input_str);
			continue;
		}

		data = atoi(input_str);
		switch (command) {
			case insert: {
				result = add_hbnode(tree, data);
				if (result == 1) {
					printf("\nINSERTED %d\n", data);
					//show_hbtree(tree, i);
					i++;
				}
				else {
					printf("\nNOT INSERTED %d\n", data);
				}
				break;
			}
			case delete: {
				result = remove_hbnode(tree, data);
				if (result == 1) {
					printf("\nDELETED %d\n", data);
					//show_hbtree(tree, i);
					i++;
				}
				else {
					printf("\nNOT DELETED %d\n", data);
				}
				break;
			}
			case find: {
				result = find_hbnode(tree, data);
				if (result == 1) {
					printf("\nFOUND %d\n", data);
				}
				else {
					printf("\nNOT FOUND %d\n", data);
				}
				break;
			}
			default: {
				break;
			}
		}

		init_input_str(input_str);
		result = scanf("%s", input_str);
	}

	delete_hbtree(tree->root);
	free(tree);

    return 1;
}

//---Funtion Implementations---//

// *** init_input_str *** //
void init_input_str(char *input_str) {
	int i;

	for (i = 0; i < 12; i++) {
		input_str[i] = '\0';
	}
}

// *** write_dotfile *** //
void write_dotfile(FILE *fptr, hbtree_node *root) {
	// Write root info
	if (root == NULL) {
		return;
	}
	fprintf(fptr, "\t%d [label=\"\\N (%d, %d,%d)\"];\n", root->data, balance_factor(root->size), root->height, root->size);

	// Write left subtree
	if (root->left != NULL) {
		fprintf(fptr, "\t%d -> %d;\n", root->data, root->left->data);
		write_dotfile(fptr, root->left);
	}

	// Write right subtree
	if (root->right != NULL) {
		fprintf(fptr, "\t%d -> %d;\n", root->data, root->right->data);
		write_dotfile(fptr, root->right);
	}
}

// *** show_hbtree *** //
// Creates a photo of the tree for debugging purposes
void show_hbtree(hbtree *tree, int photonum) {
	FILE *fptr = NULL;
	char command[36] = {'\0'};

	fptr = fopen("hbtree.dot", "w");
	if (fptr == NULL) {
		fprintf(stderr, "Error in creation of btree.dot file\n");
		exit(EXIT_FAILURE);
	}

	fprintf(fptr, "digraph G {\n");
	write_dotfile(fptr, tree->root);
	fprintf(fptr, "}");
	fclose(fptr);
	sprintf(command, "dot -Tpng hbtree.dot -o hbtree%d.png", photonum);
	system(command);
}