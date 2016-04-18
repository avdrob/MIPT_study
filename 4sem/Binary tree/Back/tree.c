#include <stdlib.h>
#include "tree.h"

struct leaf_t {
    tree_t *tree;
    struct leaf_t *parent;
    struct leaf_t *left;
    struct leaf_t *right;
    struct leaf_t *prev;
    struct leaf_t *next;
    int height;
    data_t val;
};

struct tree_t {
    leaf_t *root;
    leaf_t *first;
    leaf_t *last;
    int leaf_cnt;
    int height;
};

struct iter_t {
	tree_t *tree;
	leaf_t *cur_leaf;
};

static leaf_t *create_leaf(tree_t*, data_t);
static void subtree_dump(tree_t*, leaf_t*, FILE*, int);
static int count_height(leaf_t*);

//=====================================
//Private functions
leaf_t *create_leaf(tree_t* tree, data_t val)
{
    leaf_t *new_leaf=(leaf_t*)calloc(1, sizeof(leaf_t));
    if (!new_leaf)
        return NULL;
    new_leaf->parent=new_leaf->left=new_leaf->right=new_leaf->next=new_leaf->prev=NULL;
    new_leaf->tree=tree;
    new_leaf->val=val;
    
    return new_leaf;
}

void subtree_dump(tree_t *tree, leaf_t *leaf, FILE* outf, int depth)
{
    iter_t *it=new_iter(tree);
    if (!new_iter)
        return;
    set_iter(it, leaf);
    
    int i;
    for (i=1; i<=depth; i++)
        fprintf(outf, "   ");
    fprintf(outf, "%d: [%p] %d\n", getlh(iter_cur(it)), iter_cur(it), get_data(iter_cur(it)));
    
    if (iter_left(it))
        subtree_dump(tree, iter_left(it), outf, depth+1);
    if (iter_right(it))
        subtree_dump(tree, iter_right(it), outf, depth+1);

    delete_iter(it);
    return;
}

int count_height(leaf_t* leaf)
{
    int hl=0, hr=0;
    if (leaf->left)
        hl=count_height(leaf->left);
    if (leaf->right)
        hr=count_height(leaf->right);
    if (hl==hl==0)
        return leaf->height;
    
    return (hl>hr ? hl:hr);
}
//======================================

//======================================
//Public functions
iter_t *new_iter(tree_t *tree)
{
    iter_t *iter=(iter_t*)calloc(1, sizeof(iter_t));
    if (!iter)
        return NULL;
    iter->tree=tree;
    iter->cur_leaf=tree->root;
    
    return iter;
}

void delete_iter(iter_t *iter)
{
    iter->tree=NULL;
    iter->cur_leaf=NULL;
    free(iter);
}

leaf_t *iter_root(iter_t *iter)
{
    return iter->tree->root;
}

leaf_t *iter_begin(iter_t *iter)
{
    return iter->tree->first;
}

leaf_t *iter_end(iter_t *iter)
{
    return iter->tree->last;
}

leaf_t *iter_left(iter_t *iter)
{
    return iter->cur_leaf->left;
}

leaf_t *iter_right(iter_t *iter)
{
    return iter->cur_leaf->right;
}

leaf_t *iter_par(iter_t* iter)
{
    return iter->cur_leaf->parent;
}

leaf_t *iter_prev(iter_t *iter)
{
    return iter->cur_leaf->prev;
}

leaf_t *iter_next(iter_t *iter)
{
    return iter->cur_leaf->next;
}
    
leaf_t *iter_cur(iter_t *iter)
{
    return iter->cur_leaf;
}

void *set_iter(iter_t *iter, leaf_t *leaf)
{
    if (!leaf)
        return;
    iter->cur_leaf=leaf;
}

tree_t *new_tree()
{   
    tree_t *tree=(tree_t*)calloc(1, sizeof(tree_t));
    if (!tree)
        return NULL;
        
    leaf_t *root=create_leaf(tree, 0);
    if (!root){
        free(tree);
        return NULL;
    }
    tree->root=tree->first=tree->last=root;
    tree->root->height=0;
    tree->leaf_cnt=1;
    tree->height=0;
    
    return tree;
}

void delete_tree(tree_t *tree)
{
    delete_subtree(tree, tree->root);

    tree->root=tree->first=tree->last=NULL;
    tree->height=tree->leaf_cnt=0;
    free(tree);
}

void delete_subtree(tree_t *tree, leaf_t* leaf)
{
    if(!leaf)
        return;
    if (leaf==tree->root)
        return;
    
    if (leaf->left)
        delete_subtree(tree, leaf->left);
    if (leaf->right)
        delete_subtree(tree, leaf->right);
        
    if (leaf==leaf->parent->left)
        leaf->parent->left=NULL;
    else 
        leaf->parent->right=NULL;
    
    if (leaf->prev)
        leaf->prev->next=leaf->next;
    if (leaf->next)
        leaf->next->prev=leaf->prev;
    
    tree->leaf_cnt--;
    leaf->parent=leaf->left=leaf->right=leaf->prev=leaf->next=NULL;
    leaf->tree=NULL;
    leaf->val=0;
    leaf->height=0;
    tree->height=count_height(tree->root);
    free(leaf);
    
    return;
}

leaf_t *add_leaf(tree_t *tree, leaf_t *dest_leaf, const char mode, data_t val)
{
    if (!(mode=='r' | mode=='l'))
        return NULL;
    if ((mode=='r' && dest_leaf->right) | (mode=='l' && dest_leaf->left))
        return NULL;
        
    leaf_t *new_leaf=(leaf_t*)calloc(1, sizeof(leaf_t));
    if (!new_leaf)
        return NULL;
        
    if (mode=='r')
        dest_leaf->right=new_leaf;
    else
        dest_leaf->left=new_leaf;
    new_leaf->tree=tree;
    new_leaf->parent=dest_leaf;
    new_leaf->left=new_leaf->right=new_leaf->next=NULL;
    new_leaf->prev=tree->last;
    tree->last->next=new_leaf;
    tree->last=new_leaf;
    tree->leaf_cnt+=1;
    new_leaf->val=val;
    new_leaf->height=dest_leaf->height+1;
    if (new_leaf->height>tree->height)
        tree->height+=1;
    
    return new_leaf;
}

data_t get_data(leaf_t *leaf)
{
    return leaf->val;
}

int put_data(leaf_t *leaf, data_t val)
{
    if (!leaf)
        return 1;
    leaf->val=val;
    return 0;
}

int getlh(leaf_t *leaf)
{
    return leaf->height;
}

int gettrh(tree_t *tree)
{
    return tree->height;
}

int getlfnum(tree_t* tree)
{
    return tree->leaf_cnt;
}


void tree_dump(tree_t *tree, FILE *outf)
{
    fprintf(outf, "\n############################\n");
    fprintf(outf, "#Tree [%9p]          #\n", tree);
    fprintf(outf, "#Height =%5d             #\n", tree->height);
    fprintf(outf, "#Number of leafs =%5d    #\n", tree->leaf_cnt);
    fprintf(outf, "############################\n");
    
    subtree_dump(tree, tree->root, outf, 0);
    return;
}
