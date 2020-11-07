#ifndef _DLIST_H_
#define _DLIST_H_

// Structs //
typedef struct dlist_node_t {
    struct dlist_node_t *nxt;
    struct dlist_node_t *prev;
    int data;
} dlist_node;

typedef struct dlist_t {
    struct dlist_node_t *head;
    struct dlist_node_t *tail;
    int size;
} dlist;

// Function Prototypes //
void init_list(dlist *list);
dlist_node *find_node(dlist list, int data);
int insert_node(dlist *list, int data);
int delete_node(dlist *list, int data);

#endif