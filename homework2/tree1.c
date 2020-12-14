#include <stdio.h>
#include <stdlib.h>
#include "dlist.h"
#include "tree.h"

//-----Function Prototypes-----//
btree *construct_btree();
void find_nodes_of_level(btree_node *root, int starting_level, int target_level, dlist *list);

//------------Main-------------//
int main(int argc, char *argv[]) {
    btree *btree = construct_btree();
    dlist *list = create_list();
    int data, level;

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
        // Find and store all nodes in the level of the node given
        find_nodes_of_level(btree->root, 0, level, list);
        if (list->size == 0) {
            fprintf(stderr, "Error in find_nodes_of_level: there weren't any nodes in level %d\n", level);
            return 1;
        }

        // Print all the nodes found in that level
        printf("\nIntegers in level %d are: ", level);
        print_list(*list);
    }

    delete_list(list);
    delete_btree(btree->root);
    free(btree);

    return 0;
}

//---Funtion Implementations---//

// *** construct_btree *** //
btree *construct_btree() {
    btree *btree = create_btree();
    int data = 0;

    // Read the pre-order traversal of a binary
    // tree and construct that tree 
    scanf("%d",&data);
    while (data >= 0) {
        add_node(btree, data);
        scanf("%d", &data);
    }

    return btree;
}

// *** find_nodes_of_level *** //
void find_nodes_of_level(btree_node *root, int starting_level, int target_level , dlist *list) {
    // If we reach target level we store the value of that node, otherwise if we are in a
    // smaller level we continue to go downwards for each one of the children, giving priority
    // to the left child so that the final list will contain the level nodes in the correct order
    if (root == NULL || starting_level > target_level) {
        return;
    }
    else if (starting_level < target_level) {
        if (root->left != NULL) {
            find_nodes_of_level(root->left, starting_level + 1, target_level, list);
        }
        if (root->right != NULL) {
            find_nodes_of_level(root->right, starting_level + 1, target_level, list);
        }
    }
    else {
        insert_node(list, root->data);
    }
}