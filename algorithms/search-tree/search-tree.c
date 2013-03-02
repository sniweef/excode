#include <stdlib.h>
#include "search-tree.h"
SearchTreeNode *search(SearchTreeNode *search_tree, void *key, 
        int (*cmp)(const void *, const void *))
{
    SearchTreeNode *tree_tra = search_tree;
    while (tree_tra != NULL)
    {
        if (cmp(key, tree_tra->data) > 0)
            tree_tra = tree_tra->right;
        else if (cmp(key, tree_tra->data) < 0)
            tree_tra = tree_tra->left;
        else
            break;
    }
    return tree_tra;
}
SearchTreeNode *minimum(SearchTreeNode *search_tree)
{
    SearchTreeNode *tree_tra = search_tree;
    while (tree_tra != NULL)
        tree_tra = tree_tra->left;
    if (tree_tra == NULL)
        return NULL;
    return tree_tra;
}
SearchTreeNode *maximum(SearchTreeNode *search_tree) 
{
    SearchTreeNode *tree_tra = search_tree;
    while (tree_tra != NULL)
        tree_tra = tree_tra->right;
    if (tree_tra == NULL)
        return NULL;
    return tree_tra;
}
SearchTreeNode *predecessor(SearchTreeNode *node)
{
    if (node->left != NULL)
        return maximum(node);
    SearchTreeNode *tree_tra;
    tree_tra = node->parent;
    while (tree_tra != NULL && node == tree_tra->left)
    {
        node = tree_tra;
        tree_tra = tree_tra->parent;
    }
    return tree_tra;
}
SearchTreeNode *successor(SearchTreeNode *node)
{
    if (node->right != NULL)
        return minimum(node);
    SearchTreeNode *tree_tra;
    tree_tra = node->parent;
    while (tree_tra != NULL && node == tree_tra->right)
    {
        node = tree_tra;
        tree_tra = tree_tra->parent;
    }
    return tree_tra;
}
int insert(SearchTreeNode **search_tree, void *data,
        int (*cmp)(const void *, const void *))
{
    SearchTreeNode *new = calloc(1, sizeof(SearchTreeNode));
    if (new == NULL)
        return -1;
    new->data = data;
    new->left = NULL;
    new->right = NULL;

    SearchTreeNode *p = NULL;
    SearchTreeNode *c = *search_tree;
    while (c != NULL)
    {
        p = c;
        if (cmp(data, c->data) < 0)
            c = c->left;
        else
            c = c->right;
    }
    new->parent = p;

    if (p == NULL)
        *search_tree = new;
    else if (cmp(data, p->data) < 0)
        p->left = new;
    else 
        p->right = new;
    return 0;
}
int delete(SearchTreeNode **search_tree, SearchTreeNode *node)
{
    if (node == NULL)
        return -1;
    SearchTreeNode *delete_node;
    if (node->left == NULL || node->right == NULL)
        delete_node = node;
    else
        delete_node = successor(node); 

    SearchTreeNode *delete_node_child;
    if (delete_node->left != NULL)
        delete_node_child = delete_node->left;
    else 
        delete_node_child = delete_node->right;

    if (delete_node_child != NULL)
        delete_node_child->parent = delete_node->parent;
    if (delete_node->parent == NULL)
        *search_tree = delete_node_child;
    else if (delete_node == delete_node->parent->left)
        delete_node->parent->left = delete_node_child;
    else
        delete_node->parent->right = delete_node_child;
    
    if (delete_node != node)
        node->data = delete_node->data;
    return 0;
}

int main(int argc, const char *argv[])
{
    
    return 0;
}
