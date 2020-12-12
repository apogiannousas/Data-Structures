#include <stdio.h>
#include <stdlib.h>
#include "tree.h"

//-----------Structs-----------//
typedef struct dynamic_array_t {
    int *array;
    int size;
} dynamic_array;

//-----Function Prototypes-----//
dynamic_array *read_preorder();
btree *construct_btree(dynamic_array array);
void find_nodes_of_level(btree_node *root, int starting_level, int target_level, dynamic_array *d_array);

//------------Main-------------//
int main(int argc, char *argv[]) {
    int i, data, level;
    dynamic_array *d_array;
    btree *btree;

    // Read a preorder tree traversal and construct the binary tree
    d_array = read_preorder();
    btree = construct_btree(*d_array);
    
    // Delete the array's info
    free(d_array->array);
    d_array->array = NULL;
    d_array->size = 0;

    // Request an integer
    printf("Enter integer: ");
    scanf("%d", &data);

    // Find the integer's level. If it does not exist
    // terminate the program with error, otherwise
    // find and print all integers in the same level
    level = find_nodeLevel(btree, data);
    if (level == -1) {
        printf("%d not found!\n", data);
        return 1;
    }
    else {
        find_nodes_of_level(btree->root, 0, level, d_array);
        printf("\nIntegers in level %d are: ", level);
        for (i = 0; i < d_array->size; i++) {
            printf("%d ", d_array->array[i]);
        }
        putchar('\n');
    }

    free(d_array->array);
    free(d_array);
    delete_btree(btree->root);
    free(btree);

    return 0;
}

//---Funtion Implementations---//

// *** read_preorder *** //
dynamic_array *read_preorder() {
    dynamic_array *d_array; 
    int data = 0, *temp;

    // Allocate memory for a dynamic array
    d_array = (dynamic_array *) malloc(sizeof(dynamic_array));
    if (d_array == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    d_array->array = NULL;
    d_array->size = 0;

    // Read the pre-order traversal of a binary
    // tree and store it in an array
    scanf("%d",&data);
    while (data >= 0) {
        (d_array->size)++;
        temp = (int *) realloc(d_array->array, (d_array->size)*sizeof(int));
        if (temp == NULL) {
            perror("realloc");
            exit(EXIT_FAILURE);
        }
        d_array->array = temp;

        d_array->array[(d_array->size) - 1] = data;
        scanf("%d", &data);
    }

    return d_array;
}

// *** construct_btree *** //
btree *construct_btree(dynamic_array d_array) {
    btree *btree = create_btree();
    int i;

    for (i = 0; i < d_array.size; i++) {
        add_node(btree, d_array.array[i]);
    }

    return btree;
}

// *** find_nodes_of_level *** //
void find_nodes_of_level(btree_node *root, int starting_level, int target_level , dynamic_array *d_array) {
    int *temp;

    if (root == NULL || starting_level > target_level) {
        return;
    }
    else if (starting_level < target_level) {
        if (root->left != NULL) {
            find_nodes_of_level(root->left, starting_level + 1, target_level, d_array);
        }
        if (root->right != NULL) {
            find_nodes_of_level(root->right, starting_level + 1, target_level, d_array);
        }
    }
    else {
        (d_array->size)++;
        temp = (int *) realloc(d_array->array, (d_array->size)*sizeof(int));
        if (temp == NULL) {
            perror("realloc");
            exit(EXIT_FAILURE);
        }
        d_array->array = temp;
        d_array->array[(d_array->size) - 1] = root->data;
    }
}