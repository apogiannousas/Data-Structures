#include <stdio.h>
#include <stdlib.h>
#include "dlist.h"
#include "tree.h"

//-----Function Prototypes-----//
dlist *read_sequence();
void insertion_sort(dlist *list);
void fill_btree(dlist *list, btree *btree);
void print_preorder(btree_node *root);

//------------Main-------------//
int main(int argc, char *argv[]) {
    dlist *list = read_sequence();
    btree *btree = create_btree();

    // Check if list is empty, otherwise sort it
    if (list->size == 0) {
        fprintf(stderr, "There hasn't been given a sequence of number\n");
        return 1;
    }
    insertion_sort(list);

    // Fill the binary tree and print its pre-order traversal
    fill_btree(list, btree);
    print_preorder(btree->root);
    putchar('\n');

    delete_list(list);
    delete_btree(btree->root);
    free(btree);

    return 0;
}

//---Funtion Implementations---//

// *** read_sequence *** //
dlist *read_sequence() {
    dlist *list = create_list(); 
    int data = 0;

    // Store a sequence of numbers in a list
    scanf("%d",&data);
    while (data >= 0) {
        insert_node(list, data);
        scanf("%d", &data);
    }

    return list;
}

// *** insertion_sort *** //
void insertion_sort(dlist *list) {
	dlist_node *leftpart_node, *rightpart_node;
	int i = 1, pivot;
	
	// Check if list has at least one element
	if (list->size < 1) {
		return;
	}

	// Insert each element of the unsorted part at its right position in the 
	// sorted part. Note that sorted part is on the left before the unsorted part  
	for (rightpart_node = list->head->nxt->nxt; rightpart_node != list->tail; rightpart_node = rightpart_node->nxt, i++) {
		pivot = rightpart_node->data;
		leftpart_node = rightpart_node->prev;

		while (leftpart_node != list->head && leftpart_node->data > pivot) {
			leftpart_node->nxt->data = leftpart_node->data;
			leftpart_node = leftpart_node->prev;
		}
		leftpart_node->nxt->data = pivot;
	}
}

// *** fill_btree *** //
void fill_btree(dlist *list, btree *btree) {
	int middle = (list->size / 2) + 1;
	dlist_node *right_node = list->tail->prev;
	dlist_node *mid_node = find_node_inplace(*list, middle);
    dlist *list2;

    // If we have less than two nodes simply insert them in the tree and return
    if (list->size == 2) {
        add_node(btree, list->head->nxt->data);
        add_node(btree, list->tail->prev->data);
        return;
    }
    else if (list->size == 1) {
        add_node(btree, list->head->nxt->data);
        return;
    }
    list2 = create_list();

	// Check if middle node was found;
	if (mid_node == NULL) {
		fprintf(stderr, "Error in fill_btree: failed to find mid_node\n");
		exit(EXIT_FAILURE);
	}

	// Make the node after the node in the middle the new starting node of list2
	mid_node->nxt->prev = list2->head;
	list2->head->nxt = mid_node->nxt;

	// Make the rightmost node the new ending node of list2
	right_node->nxt = list2->tail;
	list2->tail->prev = right_node;

	// Make the node before the node in the middle the new ending node of list
	mid_node->prev->nxt = list->tail;
	list->tail->prev = mid_node->prev;

    // Detach the middle node from both lists, insert it in tree and delete it
    mid_node->nxt = NULL;
    mid_node->prev = NULL;
    add_node(btree, mid_node->data);
    free(mid_node);

	// Give both lists their actual sizes
	list2->size = list->size - middle;
	list->size = middle - 1;

	// Repeat for each of the two lists and delete list2 in the end
    fill_btree(list, btree);
    fill_btree(list2, btree);
    delete_list(list2);
}

void print_preorder(btree_node *root) {
    if (root != NULL) {
        printf("%d ", root->data);
        print_preorder(root->left);
        print_preorder(root->right);
    }
}