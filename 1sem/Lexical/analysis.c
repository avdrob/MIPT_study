#include <stdio.h>
#include <malloc.h>
#include <math.h>
#include "token_types.h"
#include <string.h>
#include "op_num.h"
#include "tree.h"

#define CODE_SIZE 200
#define BUF_TOKEN_SIZE 50

double get_n (char fst_dig);
void get_id (char dest [], char fst_ltr);
int add_symb (char dest [], char src);
void unknown_comand (char inv_str []);
tree_t* Get_G ();
tree_leaf_t* Get_Op (tree_t* pgm_tree);
tree_leaf_t* Get_Cond (tree_t* pgm_tree);
tree_leaf_t* Get_A (tree_t* pgm_tree);
tree_leaf_t* Get_E (tree_t* pgm_tree);
tree_leaf_t* Get_T (tree_t* pgm_tree);
tree_leaf_t* Get_P (tree_t* pgm_tree);
tree_leaf_t* Get_Pow (tree_t* pgm_tree);
tree_leaf_t* Get_N (tree_t* pgm_tree);


typedef struct
{
    int type;
    double data;
} token;

int TOKEN_CNT = 0, CUR_TOKEN = 0;
FILE* SOURCE = NULL;
token CODE [CODE_SIZE] = {};
int COND_FLAG = 0;

int main ()
{
    lexycal_analysis ();
    tree_t* pgm_tree = Get_G ();

    tree_dump (pgm_tree);

    return 0;
}

tree_t* Get_G ()
{
    tree_t* pgm_tree = new_tree ();

    if ((((CODE [CUR_TOKEN]).type == TYPE_OPER) && ((CODE [CUR_TOKEN + 1]).type == TYPE_OPER)) &&
        (((CODE [CUR_TOKEN]).data == OP_LESS) && ((CODE [CUR_TOKEN + 1]).data == OP_LESS)))
            CUR_TOKEN += 2;
    else
        syntax_error ();


    add_leaf (pgm_tree, pgm_tree->root, Get_Op (pgm_tree), "l");

    if ((((CODE [CUR_TOKEN]).type == TYPE_OPER) && ((CODE [CUR_TOKEN + 1]).type == TYPE_OPER)) &&
        (((CODE [CUR_TOKEN]).data == OP_MORE) && ((CODE [CUR_TOKEN + 1]).data == OP_MORE)))
            CUR_TOKEN += 2;
    else
        syntax_error ();

    if ((CODE [CUR_TOKEN]).type)
        syntax_error ();

    return pgm_tree;
}

tree_leaf_t* Get_Op (tree_t* pgm_tree)
{
    tree_leaf_t* op_leaf = new_leaf (TYPE_OPER, OP_OP);

    if (((CODE [CUR_TOKEN]).type == TYPE_OPER) && (CODE [CUR_TOKEN]).data == OP_OPEN_FIG_BR)
    {
        CUR_TOKEN++;
        add_leaf (pgm_tree, op_leaf, Get_Op (pgm_tree), "l");

        if (COND_FLAG)
            return op_leaf;

        add_leaf (pgm_tree, op_leaf, Get_Op (pgm_tree), "r");

        return op_leaf;
    }

    if (((CODE [CUR_TOKEN]).type == TYPE_OPER) && (CODE [CUR_TOKEN]).data == OP_CLOSE_FIG_BR)
    {
        CUR_TOKEN++;
        return NULL;
    }

    if (((CODE [CUR_TOKEN]).type == TYPE_OPER) && ((CODE [CUR_TOKEN]).data == OP_WHILE))
    {
        CUR_TOKEN++;
        if (((CODE [CUR_TOKEN]).type == TYPE_OPER) && ((CODE [CUR_TOKEN]).data == OP_OPEN_RND_BR))
        {
            CUR_TOKEN++;
            tree_leaf_t* whl_leaf = new_leaf (TYPE_OPER, OP_WHILE);
            add_leaf (pgm_tree, whl_leaf, Get_Cond (pgm_tree), "l");

            if (((CODE [CUR_TOKEN]).type == TYPE_OPER) && ((CODE [CUR_TOKEN]).data == OP_CLOSE_RND_BR))
            {
                CUR_TOKEN++;

                COND_FLAG++;
                add_leaf (pgm_tree, whl_leaf, Get_Op (pgm_tree), "r");
                COND_FLAG--;

                add_leaf (pgm_tree, op_leaf, whl_leaf, "l");

                if (COND_FLAG)
                    return op_leaf;

                add_leaf (pgm_tree, op_leaf, Get_Op (pgm_tree), "r");

                return op_leaf;
            }
        }
    }

    if (((CODE [CUR_TOKEN]).type == TYPE_OPER) && ((CODE [CUR_TOKEN]).data == OP_IF))
    {
        CUR_TOKEN++;
        if (((CODE [CUR_TOKEN]).type == TYPE_OPER) && ((CODE [CUR_TOKEN]).data == OP_OPEN_RND_BR))
        {
            CUR_TOKEN++;
            tree_leaf_t* if_leaf = new_leaf (TYPE_OPER, OP_IF);
            add_leaf (pgm_tree, if_leaf, Get_Cond (pgm_tree), "l");

            if (((CODE [CUR_TOKEN]).type == TYPE_OPER) && ((CODE [CUR_TOKEN]).data == OP_CLOSE_RND_BR))
            {
                CUR_TOKEN++;

                COND_FLAG++;
                add_leaf (pgm_tree, if_leaf, Get_Op (pgm_tree), "r");
                COND_FLAG--;

                add_leaf (pgm_tree, op_leaf, if_leaf, "l");

                if (COND_FLAG)
                    return op_leaf;

                add_leaf (pgm_tree, op_leaf, Get_Op (pgm_tree), "r");

                return op_leaf;
            }
        }
    }

    if (((CODE [CUR_TOKEN]).type == TYPE_OPER) && ((CODE [CUR_TOKEN]).data == OP_RET))
    {
        tree_leaf_t* ret_leaf = new_leaf (TYPE_OPER, OP_RET);
        CUR_TOKEN++;
        add_leaf (pgm_tree, ret_leaf, Get_E (pgm_tree), "l");

        if (((CODE [CUR_TOKEN]).type == TYPE_OPER) && ((CODE [CUR_TOKEN]).data == OP_SEP))
            CUR_TOKEN++;

        add_leaf (pgm_tree, op_leaf, ret_leaf, "l");

        if (COND_FLAG)
                return op_leaf;

        add_leaf (pgm_tree, op_leaf, Get_Op (pgm_tree), "r");

        return op_leaf;
    }

    if ((CODE [CUR_TOKEN]).type == TYPE_VAR)
    {
        add_leaf (pgm_tree, op_leaf, Get_A (pgm_tree), "l");

        if (((CODE [CUR_TOKEN]).type == TYPE_OPER) && ((CODE [CUR_TOKEN]).data == OP_SEP))
        {
            CUR_TOKEN++;

            if (COND_FLAG)
                return op_leaf;

            add_leaf (pgm_tree, op_leaf, Get_Op (pgm_tree), "r");

            if (((CODE [CUR_TOKEN]).type == TYPE_OPER) && ((CODE [CUR_TOKEN]).data == OP_SEP))
            {
                CUR_TOKEN++;
                return op_leaf;
            }
        }
    }

    if (!(op_leaf->left))
        return NULL;
    else
        return op_leaf;
}

tree_leaf_t* Get_Cond (tree_t* pgm_tree)
{
    tree_leaf_t* cond_leaf = new_leaf (TYPE_OPER, 0);

    add_leaf (pgm_tree, cond_leaf, Get_E (pgm_tree), "l");

    if (((CODE [CUR_TOKEN]).type == TYPE_OPER) &&
        (((CODE [CUR_TOKEN]).data == OP_LESS) || ((CODE [CUR_TOKEN]).data == OP_MORE) ||
         ((CODE [CUR_TOKEN]).data == OP_LES_EQ) || ((CODE [CUR_TOKEN]).data == OP_MOR_EQ) ||
         ((CODE [CUR_TOKEN]).data == OP_EQ) || ((CODE [CUR_TOKEN]).data == OP_NEQ)))
         {

            (cond_leaf->value).data = (CODE [CUR_TOKEN]).data;

            CUR_TOKEN++;
            add_leaf (pgm_tree, cond_leaf, Get_E (pgm_tree), "r");

            return cond_leaf;
        }

    return NULL;
}

tree_leaf_t* Get_A (tree_t* pgm_tree)
{
    if ((CODE [CUR_TOKEN]).type == TYPE_VAR)
    {
        tree_leaf_t* var_leaf = new_leaf (TYPE_VAR, (CODE [CUR_TOKEN]).data);

        CUR_TOKEN++;

        if ((CODE [CUR_TOKEN]).type == TYPE_OPER && (CODE [CUR_TOKEN]).data == OP_EQ)
        {
            tree_leaf_t* assign_leaf = new_leaf (TYPE_OPER, OP_EQ);
            add_leaf (pgm_tree, assign_leaf, var_leaf, "l");

            CUR_TOKEN++;

            add_leaf (pgm_tree, assign_leaf, Get_E (pgm_tree), "r");
            return assign_leaf;
        }
    }

    return NULL;
}

tree_leaf_t* Get_E (tree_t* pgm_tree)
{
    tree_leaf_t* expr_leaf = Get_T (pgm_tree);

    while (((CODE [CUR_TOKEN]).type == TYPE_OPER) &&
          (((CODE [CUR_TOKEN]).data == OP_PLUS) || (CODE [CUR_TOKEN]).data == OP_MINUS))
    {
        tree_leaf_t* op_leaf = new_leaf (TYPE_OPER, (CODE [CUR_TOKEN]).data);
        CUR_TOKEN++;

        add_leaf (pgm_tree, op_leaf, expr_leaf, "l");
        add_leaf (pgm_tree, op_leaf, Get_T (pgm_tree), "r");

        expr_leaf = op_leaf;
    }

    return expr_leaf;
}

tree_leaf_t* Get_T (tree_t* pgm_tree)
{
    tree_leaf_t* expr_leaf = Get_P (pgm_tree);

    while (((CODE [CUR_TOKEN]).type == TYPE_OPER) &&
          (((CODE [CUR_TOKEN]).data == OP_MUL) || (CODE [CUR_TOKEN]).data == OP_DIV))
    {
        tree_leaf_t* op_leaf = new_leaf (TYPE_OPER, (CODE [CUR_TOKEN]).data);
        CUR_TOKEN++;

        add_leaf (pgm_tree, op_leaf, expr_leaf, "l");
        add_leaf (pgm_tree, op_leaf, Get_P (pgm_tree), "r");

        expr_leaf = op_leaf;
    }

    return expr_leaf;
}

tree_leaf_t* Get_P (tree_t* pgm_tree)
{
    if ((CODE [CUR_TOKEN]).type == TYPE_OPER)
        if ((CODE [CUR_TOKEN]).data == OP_OPEN_RND_BR)
        {
            CUR_TOKEN++;
            tree_leaf_t* expr_leaf = Get_E (pgm_tree);

            if ((CODE [CUR_TOKEN]).type == TYPE_OPER && (CODE [CUR_TOKEN]).data == OP_POW)
            {
                tree_leaf_t* pow_leaf = Get_Pow (pgm_tree);
                add_leaf (pgm_tree, pow_leaf, expr_leaf, "l");

                if ((CODE [CUR_TOKEN]).type == TYPE_OPER && (CODE [CUR_TOKEN]).data == OP_CLOSE_RND_BR)
                {
                    CUR_TOKEN++;

                    if ((CODE [CUR_TOKEN]).type == TYPE_OPER && (CODE [CUR_TOKEN]).data == OP_POW)
                    {
                        tree_leaf_t* more_pow_leaf = new_leaf (TYPE_OPER, OP_POW);
                        add_leaf (pgm_tree, more_pow_leaf, pow_leaf, "l");
                        CUR_TOKEN++;
                        add_leaf (pgm_tree, more_pow_leaf, Get_E (pgm_tree), "r");

                        return more_pow_leaf;
                    }

                    return pow_leaf;
                }
            }

            if ((CODE [CUR_TOKEN]).type == TYPE_OPER && (CODE [CUR_TOKEN]).data == OP_CLOSE_RND_BR)
            {
                CUR_TOKEN++;

                if ((CODE [CUR_TOKEN]).type == TYPE_OPER && (CODE [CUR_TOKEN]).data == OP_POW)
                {
                    tree_leaf_t* pow_leaf = Get_Pow (pgm_tree);
                    add_leaf (pgm_tree, pow_leaf, expr_leaf, "l");

                    return pow_leaf;
                }

                return expr_leaf;
            }
        }

    if ((CODE [CUR_TOKEN]).type == TYPE_VAR)
    {
        tree_leaf_t* var_leaf = new_leaf (TYPE_VAR, (CODE [CUR_TOKEN]).data);
        CUR_TOKEN++;

        if ((CODE [CUR_TOKEN]).type == TYPE_OPER && (CODE [CUR_TOKEN]).data == OP_POW)
        {
            tree_leaf_t* pow_leaf = Get_Pow (pgm_tree);
            add_leaf (pgm_tree, pow_leaf, var_leaf, "l");

            return pow_leaf;
        }

        return var_leaf;
    }

    if ((CODE [CUR_TOKEN]).type == TYPE_FUNC)
    {
        CUR_TOKEN++;
        if (((CODE [CUR_TOKEN]).type == TYPE_OPER) && ((CODE [CUR_TOKEN]).data == OP_OPEN_RND_BR))
        {
            tree_leaf_t* func_leaf = new_leaf (TYPE_FUNC, (CODE [CUR_TOKEN - 1]).data);
            CUR_TOKEN++;
            add_leaf (pgm_tree, func_leaf, Get_E (pgm_tree), "l");

            if (((CODE [CUR_TOKEN]).type == TYPE_OPER) && ((CODE [CUR_TOKEN]).data == OP_CLOSE_RND_BR))
            {
                CUR_TOKEN++;
                return func_leaf;
            }
        }
    }

    if ((CODE [CUR_TOKEN]).type == TYPE_NUM)
    {
        tree_leaf_t* num_leaf = Get_N (pgm_tree);
        CUR_TOKEN++;

        if ((CODE [CUR_TOKEN]).type == TYPE_OPER && (CODE [CUR_TOKEN]).data == OP_POW)
        {
            tree_leaf_t* pow_leaf = Get_Pow (pgm_tree);
            add_leaf (pgm_tree, pow_leaf, num_leaf, "l");

            return pow_leaf;
        }

        return num_leaf;
    }

    return NULL;
}

tree_leaf_t* Get_Pow (tree_t* pgm_tree)
{
    if (((CODE [CUR_TOKEN]).type == TYPE_OPER) && (CODE [CUR_TOKEN]).data == OP_POW)
    {
        CUR_TOKEN++;
        tree_leaf_t* pow_leaf = new_leaf (TYPE_OPER, OP_POW);
        add_leaf (pgm_tree, pow_leaf, Get_P (pgm_tree), "r");

        return pow_leaf;
    }

    return NULL;
}

tree_leaf_t* Get_N (tree_t* pgm_tree)
{
    if ((CODE [CUR_TOKEN]).type = TYPE_NUM)
        return new_leaf (TYPE_NUM, (CODE [CUR_TOKEN]).data);

    return NULL;
}

int lexycal_analysis ()
{
    int TOKEN_CNT = 0;
    SOURCE = fopen ("input.txt", "r");

    char buf = "";
    #include "chk_func.h"
    #include "chk_ops.h"
    while ((buf = fgetc (SOURCE)) != EOF)
    {
        if (buf == ' ' || buf == '\t' || buf == '\n')
            continue;

        if ('0' <= buf && buf <= '9')
        {
            (CODE [TOKEN_CNT]).data = get_n (buf);
            (CODE [TOKEN_CNT++]).type = TYPE_NUM;
            continue;
        }

        char token_id [BUF_TOKEN_SIZE] = "";
        get_id (token_id, buf);

        if ((strlen (token_id) == 1) &&
            ('a' <= token_id [0] && token_id [0] <= 'z'))
            {
                (CODE [TOKEN_CNT]).data = token_id [0] - 'a';
                (CODE [TOKEN_CNT++]).type = TYPE_VAR;
                continue;
            }

        CHK_FUNC (token_id);
        CHK_OPER (token_id);

        unknown_comand (token_id);
    }

    return TOKEN_CNT;
}

double get_n (char fst_dig)
{
    double res = fst_dig - '0';
    char buf = "";

    while (('0' <= (buf = fgetc (SOURCE)) && buf <= '9') && buf != EOF)
    {
        res *= 10;
        res += buf - '0';
    }

    if (buf == '.')
    {
        int degree = 0;
        while (('0' <= (buf = fgetc (SOURCE)) && buf <= '9') && buf != EOF)
        {
            res *= 10;
            res += buf - '0';
            degree++;
        }
        res /= pow (10, degree);
    }

    ungetc (buf, SOURCE);

    return res;
}

void get_id (char dest [], char fst_ltr)
{
    char symb = "";
    char buf [1] = "";

    add_symb (dest, fst_ltr);

    if (fst_ltr == '<' || fst_ltr == '>')
        if (((symb = fgetc (SOURCE)) == '=') || (fst_ltr == '>' && symb == '<'))
        {
            add_symb (buf, symb);
            strcat (dest, buf);

            return;
        }

        else
            ungetc (symb, SOURCE);

    if (!('a' <= fst_ltr && fst_ltr <= 'z'))
        return;

    while (('a' <= (symb = fgetc (SOURCE)) && symb <= 'z') && symb != EOF)
    {
        buf [0] = '\0';
        add_symb (buf, symb);
        strcat (dest, buf);
    }

    ungetc (symb, SOURCE);

    return;
}

int add_symb (char dest [], char src)
{
    int length = strlen (dest);

    dest [length] = src;
    dest [length + 1] = '\0';

    return 0;
}

void syntax_error ()
{
    printf ("#SYNTAX ERROR: ");

    int cnt = CUR_TOKEN;
    while ((cnt >= 0) && !(((CODE [cnt]).type == TYPE_OPER) && ((CODE [cnt]).data == OP_SEP)))
        cnt--;

    cnt++;
    for (; cnt <= CUR_TOKEN; cnt++)
        display (cnt);

    printf ("\n");

    abort ();
}

void unknown_comand (char inv_str [])
{
    printf ("#UNKNOWN COMMAND: %s\n", inv_str);
    abort();
}

void display (int token_num)
{
    int cnt = 0;

    #include "chk_func.h"
    #include "chk_ops.h"

    switch ((CODE [token_num]).type)
    {
        case TYPE_VAR:
            putchar ((int) ((CODE [token_num]).data + 'a'));
            printf (" ");
            break;

        case TYPE_NUM:
            printf ("%g ", (CODE [token_num]).data);
            break;

        case TYPE_FUNC:
        {
            for (cnt = 0; cnt <= NUM_FUNC - 1; cnt++)
                if ((CODE [token_num]).data == (functions [cnt]).num)
                {
                    printf ("%s ", (functions [cnt]).str);
                    break;
                }
            break;
        }

        case TYPE_OPER:
        {
            for (cnt = 0; cnt <= NUM_OPER - 1; cnt++)
                if ((CODE [token_num]).data == (operators [cnt]).num)
                {
                    printf ("%s ", (operators [cnt]).str);
                    break;
                }
            break;
        }

        default:
            return;
    }

    return;
}

