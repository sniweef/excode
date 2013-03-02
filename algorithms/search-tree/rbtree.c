#include <stdlib.h>
#include "rbtree.h"

void left_rota(RBTreeNode **rbtree, RBTreeNode *node)
{
    RBTreeNode *node_right = node->right;
    node->right = node_right->left;
    if (node->right != NULL_NODE)
        node->right->parent = node;

    node_right->parent = node->parent;
    node->parent = node_right;
    node_right->left = node;

    RBTreeNode *node_parent = node_right->parent;
    if (node_parent == NULL_NODE)
        *rbtree = node_right;
    else if (node_parent->left == node)
        node_parent->left = node_right;
    else 
        node_parent->right = node_right;
}

void right_rota(RBTreeNode **rbtree, RBTreeNode *node)
{
    RBTreeNode *node_left = node->left;
    node->left = node_left->right;
    if (node->left != NULL_NODE)
        node->left->parent = node;

    node_left->parent = node->parent;
    node->parent = node_left;
    node_left->right = node;

    RBTreeNode *node_parent = node_left->parent;
    if (node_parent == NULL_NODE)
        *rbtree = node_left;
    else if (node_parent->left == node)
        node_parent->left = node_left;
    else
        node_parent->right = node_left;
}
void rb_insert_fixup(RBTreeNode **rbtree, RBTreeNode *node)
{
    while (node->parent->color == RED)
    {
        RBTreeNode *node_parent = node->parent;
        if (node_parent == node_parent->parent->left)
        {
            RBTreeNode *node_uncle = node_parent->parent->right;
            if (node_uncle->color == RED)
            {
                node_parent->color = BLACK;
                node_uncle->color = BLACK;
                node_parent->parent = RED;
                node = node_parent->parent;
            } else if (node == node_parent->right) {
                node = node_parent;
                left_rota(rbtree, node);
            } else {
                node_parent->color = BLACK;
                node_parent->parent->color = RED;
                right_rota(rbtree, node_parent->parent);
            }
        } else {
            RBTreeNode *node_uncle = node_parent->parent->left;
            if (node_uncle->color == RED)
            {
                node_parent->color = BLACK;
                node_uncle->color = BLACK;
                node_parent->parent = RED;
                node = node_parent->parent;
            } else if (node == node_parent->left) {
                node = node_parent;
                right_rota(rbtree, node);
            } else {
                node_parent->color = BLACK;
                node_parent->parent->color = RED;
                left_rota(rbtree, node_parent->parent);
            }
        }
    }
    *rbtree->color = BLACK;
}
int insert(RBTreeNode **rbtree, void *data,
        int (*cmp)(const void *, const void *))
{
    RBTreeNode *new = calloc(1, sizeof(RBTreeNode));
    if (new == NULL)
        return -1;
    new->data = data;
    new->left = NULL_NODE;
    new->right = NULL_NODE;
    new->color = RED;

    RBTreeNode *p = NULL_NODE;
    RBTreeNode *c = *rbtree;
    while (c != NULL_NODE)
    {
        p = c;
        if (cmp(data, c->data) < 0)
            c = c->left;
        else
            c = c->right;
    }
    new->parent = p;

    if (p == NULL_NODE)
        *rbtree = new;
    else if (cmp(data, p->data) < 0)
        p->left = new;
    else 
        p->right = new;

    rb_insert_fixup(rbtree, new);
    return 0;
}

int main(int argc, const char *argv[])
{
    
    return 0;
}
