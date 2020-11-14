#include <stdio.h>
#include <stdlib.h>
#include "dlist.h"

// *** init_list *** //
dlist *create_list() {
    dlist *new_list;
    
    // Memory allocation for list, head and tail
    new_list = (dlist *) malloc(sizeof(dlist));
    if (new_list == NULL) {
        exit(EXIT_FAILURE);
    }

    new_list->head = (dlist_node *) malloc(sizeof(dlist_node));
    if (new_list->head == NULL) {
        exit(EXIT_FAILURE);
    }

    new_list->tail = (dlist_node *) malloc(sizeof(dlist_node));
    if (new_list->tail == NULL) {
        exit(EXIT_FAILURE);
    }

    // Initialise the data field
    new_list->head->data = 0;
    new_list->head->data = 0;

    // Initialise head's and tail's next and previous nodes to build the list
    new_list->head->prev = NULL;
    new_list->head->nxt = new_list->tail;
    new_list->tail->prev = new_list->head;
    new_list->tail->nxt = NULL;
    new_list->size = 0;

    return new_list;
}

// *** delete_list *** //
void delete_list(dlist *list) {
    dlist_node *curr, *temp;

    // When list is empty just return
    if (list == NULL) {
        return;
    }
    curr = list->head->nxt;

    // Delete all internal nodes
    while (curr != list->tail) {
        temp = curr;
        curr = curr->nxt;
        free(temp);
    }

    // Delete head, tail and list
    free(list->head);
    free(list->tail);
    free(list);
}

// *** find_node *** //
dlist_node *find_node(dlist list, int data) {
    dlist_node *curr;

    // Return the node wanted or NULL if it doesn't exist
    for (curr = list.head->nxt; curr->data != data && curr != list.tail->nxt; curr = curr->nxt);
    return curr;
}

// *** insert_node *** //
void insert_node(dlist *list, int data) {
    dlist_node *new_node;

    // Construct new node
    new_node = (dlist_node *) malloc(sizeof(dlist_node));
    if (new_node == NULL) {
        exit(EXIT_FAILURE);
    }
    new_node->data = data;
    
    // Attach it at the end of the list before the tail
    new_node->prev = list->tail->prev;
    new_node->nxt = list->tail;
    new_node->nxt->prev = new_node;
    new_node->prev->nxt = new_node;
    list->size++;
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

// *** list_sum *** //
int list_sum(dlist_node *start, dlist_node *end) {
    int sum = 0;
    dlist_node *curr;

    for (curr = start; curr != end->nxt; curr = curr->nxt) {
        sum += curr->data;
    }

    return sum;
}

// *** print_list *** //
void print_list(dlist list) {
    dlist_node *curr;

    for (curr = list.head->nxt; curr != list.tail; curr = curr->nxt) {
        printf("%d ", curr->data);
    }

    putchar('\n');
}