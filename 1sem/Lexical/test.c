#include "tree.h"

int main ()
{
    tree_t* tr = new_tree ();
    tree_leaf_t* le = new_leaf (333, 444);

    add_leaf (tr, tr->root, le, "l");

    tree_dump (tr);

    delete_leaf (tr, tr->root->left);
    tree_dump (tr);

    return 0;
}
