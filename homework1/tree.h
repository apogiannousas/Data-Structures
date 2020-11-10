#ifndef _TREE_H_
#define _TREE_H_
#include <stdbool.h>

//-----------Structs-----------//
typedef struct btree_node_t {
    struct btree_node_t *parent;
    struct btree_node_t *left;
    struct btree_node_t *right;
    int data;
} btree_node;

typedef struct btree_t {
    struct btree_node_t *root;
    int size;
} btree;

//-----Function Prototypes-----//
btree *create_btree();
void delete_btree(btree_node *root);
btree_node *find_treeNode(btree *btree, int data);
btree_node *find_maxFromLeftSubtree(btree_node *root);
btree_node *find_minFromRightSubtree(btree_node *root);
bool isRightChild(btree_node *node);
bool isLeftChild(btree_node *node);
int add_node(btree *btree, int data);
int remove_node(btree *btree, int data);

#endif