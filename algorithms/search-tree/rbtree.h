#ifndef RBTREE_H
#define RBTREE_H

#define RED     0
#define BLACK   1

typedef int Color;
typedef struct _rbtree {
    struct _rbtree *left;
    struct _rbtree *right;
    struct _rbtree *parent;
    Color color;
    void *data;
} RBTreeNode;

RBTreeNode null_node = {.color = BLACK};
#define NULL_NODE &null_node

#endif /* end of include guard: RBTREE_H */
