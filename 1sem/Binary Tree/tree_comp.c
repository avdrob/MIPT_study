//{==========================================================================
//! @file tree_comp.c
//! @date 7.12.2013
//! @author Andrey Drobyshev <immortalguardian1@gmail.com>
//! @version 1.0
//!
//! This program calculates complete bracket expression and dumps it back.
//}==========================================================================

#include "tree.h"
#include <stdio.h>
#include "leaf_types.h"
#include "stckcount.h"
#include <string.h>

#define BUF_SIZE 100

char EXP_STRING [BUF_SIZE] = "";
char* EXPR_PTR = EXP_STRING;


//{==========================================================================
//! Function get_leaf - creates new tree leaf using bracket dump.
//!
//! @param      tree       Pointer to the tree.
//!
//! @return     Pointer to the leaf.
//}===========================================================================
tree_leaf_t* get_leaf (tree_t* const tree);

//{==========================================================================
//! Function is_op - checks out if the current symbol is an operator.
//!
//! @param      symb    The symbol.
//!
//! @return     1 if it's operator;
//!             0 if it's not.
//}===========================================================================
int is_op (char symb);

//{==========================================================================
//! Function get_n - converts a sequence of digits into integer.
//!
//! @return     Number.
//}===========================================================================
double get_n ();

//{==========================================================================
//! Function valid - checks out if the input string is valid.
//!
//! @return     Nothing to be returned.
//!
//! @note       In case of invalid input the program will be aborted.
//}===========================================================================
void valid ();

//{==========================================================================
//! Function syntax_error - aborts the program without any remorse.
//}===========================================================================
void syntax_error ();

//{==========================================================================
//! Function calculate - calculates the expression using stack.
//!
//! @return     Result of calculation.
//}===========================================================================
elem_t calculate (stack_t* calc_stack, const tree_leaf_t* tree_leaf);

//{==========================================================================
//! Function bracket_dump - dumps the tree into complete bracket expression.
//!
//! @param      tree_leaf   The leaf that should be dumped with its subtree.
//! @param      output      Pointer to the pointer to output file.
//!
//! @return     Nothing to be returned.
//}===========================================================================
void bracket_dump (const tree_leaf_t* tree_leaf, FILE** output);

int main ()
{
    FILE* input = fopen ("input.txt", "r");
    fscanf (input, "%s", EXP_STRING);
    valid ();
    fclose (input);

    tree_t* tree = new_tree ();
    add_leaf (tree, tree->root, get_leaf (tree), "l");

    FILE* output = fopen ("output.txt", "w");
    stack_t* calc_stack = stack_new (BUF_SIZE / 2);
    elem_t res = calculate (calc_stack, tree->root->left);
    fprintf (output, "%g\n", res);

    bracket_dump (tree->root->left, &output);
    fclose (output);

    tree_dump (tree);

    return 0;
}

tree_leaf_t* get_leaf (tree_t* tree)
{
    tree_leaf_t* leaf = new_leaf (0, 0);

    if (*EXPR_PTR == '(')
    {
        EXPR_PTR++;

        add_leaf (tree, leaf, get_leaf (tree), "l");

        if (is_op (*EXPR_PTR))
        {
            leaf->value.type = TYPE_OP;
            leaf->value.data = *EXPR_PTR;
            EXPR_PTR += 2;
        }

        add_leaf (tree, leaf, get_leaf (tree), "r");
    }

    if (('0' <= *EXPR_PTR) && (*EXPR_PTR <= '9'))
    {
        int num = get_n ();

        leaf->value.type = TYPE_NUM;
        leaf->value.data = num;
    }

    if (*EXPR_PTR == ')' || *EXPR_PTR == '\0')
    {
        if (*EXPR_PTR == '\0' && leaf->left)
            leaf = leaf->left;

        EXPR_PTR++;
        return leaf;
    }

}

int is_op (char symb)
{
    return (symb == '+' || symb == '-' || symb == '*' || symb == '/');
}

double get_n ()
{
    double res = 0;
    while ('0' <= *EXPR_PTR && *EXPR_PTR <= '9')
    {
        res *= 10;
        res += *EXPR_PTR - '0';
        EXPR_PTR++;
    }

    return res;
}

void valid ()
{
    int cnt = 0;
    stack_t* bracket_stack = stack_new (BUF_SIZE / 2);

    for (cnt = 0; cnt <= strlen (EXP_STRING) - 1; cnt++)
    {
        assert (cnt <= strlen (EXP_STRING) - 1);

        if (!(EXP_STRING [cnt] == '(' || EXP_STRING [cnt] == ')' ||
             ('0' <= EXP_STRING [cnt] && EXP_STRING [cnt] <= '9') ||
             is_op (EXP_STRING [cnt])))

             syntax_error ();

        if ('0' <= EXP_STRING [cnt] && EXP_STRING [cnt] <= '9')
            if (!((('0' <= EXP_STRING [cnt - 1] && EXP_STRING [cnt - 1] <= '9') || EXP_STRING [cnt - 1] == '(') &&
                (('0' <= EXP_STRING [cnt + 1] && EXP_STRING [cnt + 1] <= '9') || EXP_STRING [cnt + 1] == ')')))

                syntax_error ();

        if (EXP_STRING [cnt] == '(')
            if (!((EXP_STRING [cnt + 1] == '(' || ('0' <= EXP_STRING [cnt + 1] && EXP_STRING [cnt + 1] <= '9')) &&
                 (EXP_STRING [cnt - 1] == '(' || is_op (EXP_STRING [cnt - 1]) || cnt == 0)))
                syntax_error ();
            else
                stack_push (bracket_stack, 1);

        if (EXP_STRING [cnt] == ')')
            if (!((EXP_STRING [cnt + 1] == ')') || (EXP_STRING [cnt + 1] == '\0') || is_op (EXP_STRING [cnt + 1])) &&
                 (('0' <= EXP_STRING [cnt - 1] && EXP_STRING [cnt - 1] <= '9')  || (EXP_STRING [cnt - 1] == ')')))
                 syntax_error ();
            else
            {
                elem_t buf = 0;

                if (stack_empty (bracket_stack))
                    syntax_error ();

                stack_pop (bracket_stack, &buf);
            }
    }

    if (!stack_empty (bracket_stack))
        syntax_error ();

    return;
}

void syntax_error ()
{
    printf ("#SYNTAX ERROR\n");
    printf ("#PROGRAM WAS UNEXPECTEDLY ABORTED\n");

    abort ();
}

elem_t calculate (stack_t* calc_stack, const tree_leaf_t* tree_leaf)
{
    if (tree_leaf->value.type == TYPE_NUM)
        return tree_leaf->value.data;

    if (tree_leaf->value.type == TYPE_OP)
    {
        stack_push (calc_stack, calculate (calc_stack, tree_leaf->left));
        stack_push (calc_stack, calculate (calc_stack, tree_leaf->right));

        switch ((int) tree_leaf->value.data)
        {
            case '+':
            {
                stack_add (calc_stack);
                break;
            }

            case '-':
            {
                stack_sub (calc_stack);
                break;
            }

            case '*':
            {
                stack_mul (calc_stack);
                break;
            }

            case '/':
            {
                stack_div (calc_stack);
                break;
            }
        }

        elem_t buf = 0;
        stack_pop (calc_stack, &buf);

        return buf;
    }
}

void bracket_dump (const tree_leaf_t* tree_leaf, FILE** output)
{
    if (!tree_leaf)
        return;

    fprintf (*output, "(");
    bracket_dump (tree_leaf->left, output);

    if (tree_leaf->value.type == TYPE_NUM)
        fprintf (*output, "%g", tree_leaf->value.data);
    if (tree_leaf->value.type == TYPE_OP)
        fprintf (*output, "%c", (char) tree_leaf->value.data);

    bracket_dump (tree_leaf->right, output);
    fprintf (*output, ")");

    return;
}
