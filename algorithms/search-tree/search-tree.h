#ifndef SEARCH_TREE_H
#define SEARCH_TREE_H

typedef struct _search_tree {
    void               *data;
    struct _search_tree *parent;
    struct _search_tree *left;
    struct _search_tree *right;
} SearchTreeNode;

SearchTreeNode *search(SearchTreeNode *search_tree, void *key, 
        int (*cmp)(const void *, const void *));
SearchTreeNode *minimum(SearchTreeNode *search_tree);
SearchTreeNode *maximum(SearchTreeNode *search_tree);
SearchTreeNode *predecessor(SearchTreeNode *node);
SearchTreeNode *successor(SearchTreeNode *node);
int insert(SearchTreeNode **search_tree, void *data,
        int (*cmp)(const void *, const void *));
int delete(SearchTreeNode **search_tree, SearchTreeNode *node);

#endif /* end of include guard: SEARCH_TREE_H */
