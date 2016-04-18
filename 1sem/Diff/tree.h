//{=======================================================================
//! @file tree.h
//! @date 7.12.2013
//! @author Andrey Drobyshev <immortalguardian1@gmail.com>
//! @version 1.0
//!
//! This library contains a few functions for working with bynary tree.
//}=======================================================================

#include "memloc.h"
#include <string.h>
#include "leaf_types.h"

#define TREESIZE 20

typedef struct
{
    int type;
    double data;
} value_t;

typedef struct tree_leaf_t
{
    struct tree_leaf_t* parent;
    struct tree_leaf_t* left;
    struct tree_leaf_t* right;
    value_t value;
} tree_leaf_t;

typedef struct
{
    tree_leaf_t* root;
    int tree_size;
} tree_t;


//{==========================================================================
//! Function new_tree - create tree of TREESIZE.
//!
//! @return     Pointer to the tree.
//}===========================================================================
tree_t* new_tree ();

//{==========================================================================
//! Function delete_tree - delete the tree.
//!
//! @param      tree       Pointer to the pointer to the tree.
//!
//! @return     0 if it has been done;
//!             1 if the tree is not ok, so it's impossible to delete it.
//}===========================================================================
int delete_tree (tree_t** tree);

//{==========================================================================
//! Function add_leaf - add new leaf to the tree.
//!
//! @param      tree        Pointer to the tree.
//! @param      dest_leaf   The leaf new leaf should be attached to.
//! @param      extra_leaf  Thnew leaf.
//! @param      mode        Indicates the way new leaf should be attached:
//!                         as a left kid or as a right.
//!
//! @return     Pointer to the new leaf;
//!             1 if there's no such a leaf or such a dest_leaf or
//!                 such an extra_leaf;
//!             0 if it's impossible to attach the leaf to needed place.
//}===========================================================================
tree_leaf_t* add_leaf (tree_t* tree, tree_leaf_t* dest_leaf, tree_leaf_t* extra_leaf, const char* mode);

//{==========================================================================
//! Function new_leaf - creates a new leaf with particular data.
//!
//! @param      leaf_type   Type of new leaf.
//! @param      leaf_data   New leaf data.
//!
//! @return     pointer to the new leaf.
//}===========================================================================
tree_leaf_t* new_leaf (int leaf_type, double leaf_data);

//{==========================================================================
//! Function delete_leaf - delete the leaf and all of its subtree.
//!
//! @param      tree        Pointer to the tree.
//! @param      tree_leaf   The leaf should be removed.
//!
//! @return     0 if it's ok;
//!             1 if it's impossible to delete.
//}===========================================================================
int delete_leaf (tree_t* tree, tree_leaf_t* tree_leaf);

//{==========================================================================
//! Function leaf_ok - chechs out if the leaf is ok.
//!
//! @param      tree        Pointer to the tree.
//! @param      tree_leaf   Pointer to the leaf.
//!
//! @return     1 if it's ok;
//!             0 if it isn't.
//}===========================================================================
int leaf_ok (tree_t* tree, tree_leaf_t* tree_leaf);


//{==========================================================================
//! Function tree_ok - chechs out if the leaf and its subtree are ok.
//!
//! @param      tree        Pointer to the tree.
//! @param      tree_leaf   Pointer to the leaf.
//!
//! @return     1 if it's ok;
//!             0 if it isn't.
//}===========================================================================
int tree_ok (tree_t* tree, tree_leaf_t* tree_leaf);

//{==========================================================================
//! Function tree_dump - prints out info about tree.
//!
//! @param      tree        Pointer to the tree.
//!
//! @return     Nothing to be returned.
//}===========================================================================
void tree_dump (tree_t* tree);

//{==========================================================================
//! Function leaf_dump - prints out info about tree leaf.
//!
//! @param      tree        Pointer to the tree.
//! @param      tree_leaf   Pointer to the leaf.
//! @param      depth       Indicates how deep the level the leaf locates.
//!
//! @return     Nothing to be returned.
//}===========================================================================
void leaf_dump (tree_t* tree, tree_leaf_t* tree_leaf, int depth);

//{==========================================================================
//! Function tree_size - number of tree leafs.
//!
//! @param      tree        Pointer to the tree.
//!
//! @return     Number of leafs.
//}===========================================================================
int tree_size (tree_t* tree);

//{==========================================================================
//! Function copy_tree - copies a leaf with its subtree.
//!
//! @param      tree        Pointer to the tree.
//! @param      leaf        Pointer to the leaf.
//!
//! @return     Popinter to the copied leaf.
//}===========================================================================
tree_leaf_t* copy_leaf (tree_t* tree, tree_leaf_t* leaf);

tree_t* new_tree ()
{
    tree_t* tree = (tree_t*) calloc (1, sizeof (tree_t));
    set_mem (TREESIZE, sizeof (tree_leaf_t));
    tree_leaf_t* tree_leaf = seize_mem (sizeof (tree_leaf_t));
    tree->root = tree_leaf;
    tree_leaf->left = tree_leaf->parent = tree_leaf->right = NULL;
    tree_leaf->value.type = tree_leaf->value.data = 0;
    tree->tree_size = 1;

    return tree;
}

int delete_tree (tree_t** tree)
{
    if (!tree_ok ((*tree), (*tree)->root))
        return 1;

    dispose_mem (sizeof (tree_leaf_t));
    delete_leaf (*tree, (*tree)->root);

    (*tree)->tree_size = 0;
    (*tree)->root = NULL;
    free (*tree);
    (*tree) = NULL;

    return ((*tree) && 1);
}

tree_leaf_t* add_leaf (tree_t* tree, tree_leaf_t* dest_leaf, tree_leaf_t* extra_leaf, const char* mode)
{
    if ((!tree) || (!dest_leaf) || (!extra_leaf))
        return 1;

    if ((strcmp (mode, "l")) && (strcmp (mode, "r")))
        return NULL;

    if (!strcmp (mode, "l"))
    {
        if (!dest_leaf->left)
        {
            dest_leaf->left = extra_leaf;
            extra_leaf->parent = dest_leaf;
        }

        else
            return NULL;
    }

    else
    {
        if (!dest_leaf->right)
        {
            dest_leaf->right = extra_leaf;
            extra_leaf->parent = dest_leaf;
        }

        else
            return NULL;
    }

    (tree->tree_size)++;

    return extra_leaf;
}

tree_leaf_t* new_leaf (int leaf_type, double leaf_data)
{
    tree_leaf_t* leaf = seize_mem (sizeof (tree_leaf_t));

    if (!leaf)
    {
        set_mem (TREESIZE, sizeof (tree_leaf_t));
        leaf = seize_mem (sizeof (tree_leaf_t));
    }

    leaf->value.data = leaf_data;
    leaf->value.type = leaf_type;
    leaf->left = leaf->parent = leaf->right = NULL;

    return leaf;
}

int delete_leaf (tree_t* tree, tree_leaf_t* tree_leaf)
{
    if (tree_leaf->left)
        delete_leaf (tree, tree_leaf->left);
    if (tree_leaf->right)
        delete_leaf (tree, tree_leaf->right);

    if (tree->root != tree_leaf)
    {
        if (tree_leaf->parent->left == tree_leaf)
            tree_leaf->parent->left = NULL;
        if (tree_leaf->parent->right == tree_leaf)
            tree_leaf->parent->right = NULL;
    }

    tree_leaf->left = tree_leaf->right = NULL;
    tree_leaf->value.data = tree_leaf->value.type = 0;
    int flag = tree_leaf->parent->left && tree_leaf->parent->right;
    tree_leaf->parent = NULL;
    (tree->tree_size)--;
    free_mem (tree_leaf, sizeof (tree_leaf_t));

    return flag;
}

int leaf_ok (tree_t* tree, tree_leaf_t* tree_leaf)
{
    if ((!tree_leaf->parent && !(tree_leaf == tree->root)) ||
         (tree_leaf->parent && !((tree_leaf->parent->left == tree_leaf) || (tree_leaf->parent->right == tree_leaf))))
        return 0;

    if ((tree_leaf->left && !(tree_leaf->left->parent == tree_leaf)) ||
        (tree_leaf->right && !(tree_leaf->right->parent == tree_leaf)))
        return 0;

    return 1;
}

int tree_ok (tree_t* tree, tree_leaf_t* tree_leaf)
{
    if ((!tree_leaf->left) && (!tree_leaf->right))
        return leaf_ok (tree, tree_leaf);

    return ((tree_leaf->left && tree_leaf->right && tree_ok (tree, tree_leaf->left) && tree_ok (tree, tree_leaf->right)) ||
           (!tree_leaf->left && tree_leaf->right && tree_ok (tree, tree_leaf->right)) ||
           (!tree_leaf->right && tree_leaf->left && tree_ok (tree, tree_leaf->left)) && leaf_ok (tree, tree_leaf));
}

void tree_dump (tree_t* tree)
{
    if (!tree)
    {
        printf ("#ERROR! Tree doesn't exist\n");
        return;
    }

    printf ("#Tree [%7x] is ", tree);

    if (tree_ok (tree, tree->root))
        printf ("ok\n");
    else
        printf ("BAD!\n");

    printf ("#Tree consists of %d leafs\n\n", tree->tree_size - 1);
    leaf_dump (tree, tree->root, 0);
    printf ("\n");

    return;
}

void leaf_dump (tree_t* tree, tree_leaf_t* tree_leaf, int depth)
{
    int cnt = 0;
    for (cnt = 1; cnt <= 3 * depth; cnt++)
        printf (" ");

    printf ("[%7x]: %d %g\n", tree_leaf, tree_leaf->value.type, tree_leaf->value.data);

    if (tree_leaf->left)
        leaf_dump (tree, tree_leaf->left, depth + 1);

    if (tree_leaf->right)
        leaf_dump (tree, tree_leaf->right, depth + 1);

    return;
}

int tree_size (tree_t* tree)
{
    if (!tree)
        return 0;

    else
        return tree->tree_size;
}

tree_leaf_t* copy_leaf (tree_t* tree, tree_leaf_t* leaf)
{
    tree_leaf_t* res_leaf = new_leaf ((leaf->value).type, (leaf->value).data);

    if (leaf->left)
        add_leaf (tree, res_leaf, copy_leaf (tree, leaf->left), "l");
    if (leaf->right)
        add_leaf (tree, res_leaf, copy_leaf (tree, leaf->right), "r");

    return res_leaf;
}
