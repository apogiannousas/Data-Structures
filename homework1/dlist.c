#include <stdlib.h>
#include "dlist.h"

// *** init_list *** //
void init_list(dlist *list) {
    // Memory allocation for list, head and tail
    list = (dlist *) malloc(sizeof(dlist));
    list->head = (dlist_node *) malloc(sizeof(dlist_node));
    list->tail = (dlist_node *) malloc(sizeof(dlist_node));
    // Initialise the data field
    list->head->data = 0;
    list->head->data = 0;

    // Initialise head's and tail's next and previous nodes to build the list
    list->head->prev = NULL;
    list->head->nxt = list->tail;
    list->tail->prev = list->head;
    list->tail->nxt = NULL;
    list->size = 0;
}

// *** find_node *** //
dlist_node *find_node(dlist list, int data) {
    dlist_node *curr;

    // Return the node wanted or NULL if it doesn't exist
    for (curr = list.head->nxt; curr->data != data && curr != list.tail->nxt; curr = curr->nxt);
    return curr;
}

// *** insert_node *** //
int insert_node(dlist *list, int data) {
    dlist_node *new_node;
    
    // Check if node already exists
    if (find_node(*list, data) != NULL) {
        return 0;
    }

    // Construct new node
    new_node = (dlist_node *) malloc(sizeof(dlist_node));
    new_node->data = data;
    // Attach it at the end of the list before the tail
    new_node->prev = list->tail->prev;
    new_node->nxt = list->tail;
    new_node->nxt->prev = new_node;
    new_node->prev->nxt = new_node;
    list->size++;

    return 1;
}

// *** delete_node *** //
int delete_node(dlist *list, int data) {
    dlist_node *wanted_node = find_node(*list, data);

    // Check if node doesn't exist
    if (wanted_node == NULL) {
        return 0;
    }

    // Remove it from the list
    wanted_node->prev->nxt = wanted_node->nxt;
    wanted_node->nxt->prev = wanted_node->prev;
    list->size--;
    // Delete node from memory
    free(wanted_node);

    return 1;
}