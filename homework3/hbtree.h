#ifndef _HBTREE_H_
#define _HBTREE_H_
#include <stdbool.h>
#define max(a,b) ((a) > (b) ? (a) : (b))

//-----------Structs-----------//
typedef struct hbtree_node_t {
    struct hbtree_node_t *parent;
    struct hbtree_node_t *left;
    struct hbtree_node_t *right;
    int data;
    int height;
    int size;
} hbtree_node;

typedef struct hbtree_t {
    struct hbtree_node_t *root;
    int size;
} hbtree;

//-----Function Prototypes-----//
hbtree *create_hbtree();
void delete_hbtree(hbtree_node *root);
hbtree_node *find_hbtreeNode(hbtree *btree, int data);
int find_hbnode(hbtree *btree, int data);
hbtree_node *find_rightmost_lefthbnode(hbtree_node *root);
hbtree_node *find_leftmost_righthbnode(hbtree_node *root);
int find_hbnodeLevel(hbtree *btree, int data);
bool isRightHbChild(hbtree_node *node);
bool isLeftHbChild(hbtree_node *node);
int balance_factor(int size);
int add_hbnode(hbtree *btree, int data);
int remove_hbnode(hbtree *btree, int data);
void print_hbtree_preorder(hbtree_node *root);

#endif