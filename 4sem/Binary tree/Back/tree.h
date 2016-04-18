#include <stdio.h>

typedef struct leaf_t leaf_t;
typedef struct tree_t tree_t;
typedef struct iter_t iter_t;
typedef int data_t;

tree_t *new_tree();
void delete_tree(tree_t*);
leaf_t *add_leaf(tree_t*, leaf_t*, const char, data_t);
void delete_subtree(tree_t*, leaf_t*);
void tree_dump(tree_t*, FILE*);
data_t get_data(leaf_t*);
int put_data(leaf_t*, data_t);
int getlh(leaf_t*);
int gettrh(tree_t*);
int getlfnum(tree_t*);

iter_t *new_iter(tree_t*);
void delete_iter(iter_t*);
leaf_t *iter_root(iter_t*);
leaf_t *iter_begin(iter_t*);
leaf_t *iter_end(iter_t*);
leaf_t *iter_left(iter_t*);
leaf_t *iter_right(iter_t*);
leaf_t *iter_par(iter_t*);
leaf_t *iter_prev(iter_t*);
leaf_t *iter_next(iter_t*);
leaf_t *iter_cur(iter_t*);
void *set_iter(iter_t*, leaf_t*);
