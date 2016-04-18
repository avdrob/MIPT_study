#include "tree.h"

#define PAR 20

int main()
{
    tree_t *tr=new_tree();
    iter_t *it=new_iter(tr);
    int i=0;
    char mode[2]={'l', 'r'};
    
    //crearing left branch, filling it with data
    set_iter(it, iter_root(it));
    set_iter(it, add_leaf(tr, iter_cur(it), 'l', i));
    for (i=1; i<=PAR; i++)
        set_iter(it, add_leaf(tr, iter_cur(it), mode[i%2], i));
        
    //creating right branch
    set_iter(it, iter_root(it));
    set_iter(it, add_leaf(tr, iter_cur(it), 'r', i));    
    for (i=1; i<=PAR; i++)
        set_iter(it, add_leaf(tr, iter_cur(it), mode[i%2], i));
        
    //going through the list of leafs in direct order
    set_iter(it, iter_begin(it));
    for (i=0; i<getlfnum(tr); i++){
        put_data(iter_cur(it), getlh(iter_cur(it)));
        set_iter(it, iter_next(it));
    }
    
    //going through the list of leafs in reverse order
    set_iter(it, iter_end(it));
    for (i=0; i<getlfnum(tr); i++){
        put_data(iter_cur(it), gettrh(tr)-getlh(iter_cur(it)));
        set_iter(it, iter_prev(it));
    }
    
    //going up through the tree
    set_iter(it, iter_end(it));
    while (iter_cur(it)!=iter_root(it))
        set_iter(it, iter_par(it));
    
    //putting data into non-existing leaf
    set_iter(it, iter_end(it));
    put_data(iter_next(it), 0);
    
    //trying to follow non-existing leafs in the list
    set_iter(it, iter_next(it));
    
    //attaching wrong leafs
    add_leaf(tr, iter_cur(it), 0, 0);
    set_iter(it, iter_root(it));
    add_leaf(tr, iter_cur(it), 'r', 0);
    
    //trying to delete subtree with root
    delete_subtree(tr, iter_cur(it));
    
    //testing iterator funcs
    set_iter(it, iter_root(it));
    set_iter(it, iter_right(it));
    set_iter(it, iter_root(it));
    set_iter(it, iter_left(it));
    
    delete_subtree(tr, iter_next(it));
    
    //trying to delete subtree with non-existing leaf in the root
    set_iter(it, iter_end(it));
    delete_subtree(tr, iter_next(it));
    
    //dumping tree
    tree_dump(tr, stdout);
    
    //deleting tree
    delete_tree(tr);
    
    return 0;
}
