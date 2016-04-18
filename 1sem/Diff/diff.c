#include <stdio.h>
#include "tree.h"
#include "token_types.h"
#include "op_num.h"
#include "func_num.h"
#include <math.h>

#define BUF_TOKEN_SIZE 50
#define CODE_SIZE 200

tree_leaf_t* diff (tree_t* source_tree, tree_leaf_t* diff_leaf);
tree_leaf_t* tree_const (tree_t* source_tree, tree_leaf_t* leaf);
tree_leaf_t* del_oper (tree_t* source_tree, tree_leaf_t* leaf);
tree_leaf_t* Get_E (tree_t* pgm_tree);
tree_leaf_t* Get_T (tree_t* pgm_tree);
tree_leaf_t* Get_P (tree_t* pgm_tree);
tree_leaf_t* Get_Pow (tree_t* pgm_tree);
tree_leaf_t* Get_N (tree_t* pgm_tree);
void lexycal_analysis ();
double get_n (char fst_dig);
void get_id (char dest [], char fst_ltr);
int add_symb (char dest [], char src);
void unknown_comand (char inv_str []);
void syntax_error ();
void display (int token_num);
void leaf_out (tree_leaf_t* leaf);
int get_prior (tree_leaf_t* leaf);
void Dmitry_Vladimirovich ();
int any_num (tree_leaf_t* leaf);

typedef struct
{
    int type;
    double data;
} token;

int TOKEN_CNT = 0, CUR_TOKEN = 0;
FILE* SOURCE = NULL;
FILE* OUTPUT = NULL;
FILE* PATTERN = NULL;
token CODE [CODE_SIZE] = {};
int CHNG_FLAG = 0;

int main ()
{
    SOURCE = fopen ("diff.txt", "r");
    OUTPUT = fopen ("diff.tex", "w");
    PATTERN = fopen ("pattern.tex", "r");

    lexycal_analysis ();
    Dmitry_Vladimirovich ();

    fclose (SOURCE);
    fclose (OUTPUT);
    fclose (PATTERN);

    return 0;
}

tree_leaf_t* diff (tree_t* diff_tree, tree_leaf_t* diff_leaf)
{
    if (((diff_leaf->value).type == TYPE_NUM) ||
        ((diff_leaf->value).type == TYPE_VAR) && ((diff_leaf->value).data != ('x' - 'a')))
    {
        tree_leaf_t* ret_leaf = new_leaf (TYPE_NUM, 0);
        return ret_leaf;
    }

    if (((diff_leaf->value).type == TYPE_VAR) && ((diff_leaf->value).data == 'x' - 'a'))
    {
        tree_leaf_t* ret_leaf = new_leaf (TYPE_NUM, 1);
        return ret_leaf;
    }

    if (((diff_leaf->value).type == TYPE_OPER) &&
        ((diff_leaf->value).data == OP_PLUS || (diff_leaf->value).data == OP_MINUS))
    {
        tree_leaf_t* ret_leaf = new_leaf (TYPE_OPER, (diff_leaf->value).data);
        add_leaf (diff_tree, ret_leaf, diff (diff_tree, diff_leaf->left), "l");
        add_leaf (diff_tree, ret_leaf, diff (diff_tree, diff_leaf->right), "r");

        return ret_leaf;
    }

    if (((diff_leaf->value).type == TYPE_OPER) &&
        ((diff_leaf->value).data == OP_MUL))
    {
        tree_leaf_t* plus_leaf = new_leaf (TYPE_OPER, OP_PLUS);
        tree_leaf_t* mul1 = new_leaf (TYPE_OPER, OP_MUL);
        tree_leaf_t* mul2 = new_leaf (TYPE_OPER, OP_MUL);

        add_leaf (diff_tree, mul1, diff (diff_tree, diff_leaf->left), "l");
        add_leaf (diff_tree, mul1, copy_leaf (diff_tree, diff_leaf->right), "r");
        add_leaf (diff_tree, mul2, diff (diff_tree, diff_leaf->right), "l");
        add_leaf (diff_tree, mul2, copy_leaf (diff_tree, diff_leaf->left), "r");

        add_leaf (diff_tree, plus_leaf, mul1, "l");
        add_leaf (diff_tree, plus_leaf, mul2, "r");

        return plus_leaf;
    }

    if (((diff_leaf->value).type == TYPE_OPER) &&
        ((diff_leaf->value).data == OP_DIV))
    {
        tree_leaf_t* div_leaf = new_leaf (TYPE_OPER, OP_DIV);
        tree_leaf_t* minus_leaf = new_leaf (TYPE_OPER, OP_MINUS);
        tree_leaf_t* pow_leaf = new_leaf (TYPE_OPER, OP_POW);
        tree_leaf_t* mul1 = new_leaf (TYPE_OPER, OP_MUL);
        tree_leaf_t* mul2 = new_leaf (TYPE_OPER, OP_MUL);

        add_leaf (diff_tree, mul1, diff (diff_tree, diff_leaf->left), "l");
        add_leaf (diff_tree, mul1, copy_leaf (diff_tree, diff_leaf->right), "r");
        add_leaf (diff_tree, mul2, diff (diff_tree, diff_leaf->right), "l");
        add_leaf (diff_tree, mul2, copy_leaf (diff_tree, diff_leaf->left), "r");

        add_leaf (diff_tree, minus_leaf, mul1, "l");
        add_leaf (diff_tree, minus_leaf, mul2, "r");

        tree_leaf_t* two_leaf = new_leaf (TYPE_NUM, 2);
        add_leaf (diff_tree, pow_leaf, two_leaf, "r");
        add_leaf (diff_tree, pow_leaf, copy_leaf (diff_tree, diff_leaf->right), "l");

        add_leaf (diff_tree, div_leaf, minus_leaf, "l");
        add_leaf (diff_tree, div_leaf, pow_leaf, "r");

        return div_leaf;
    }

    if (((diff_leaf->value).type == TYPE_OPER) &&
        ((diff_leaf->value).data == OP_POW))
    {
        tree_leaf_t* mul_leaf = new_leaf (TYPE_OPER, OP_MUL);
        tree_leaf_t* pow_leaf = new_leaf (TYPE_OPER, OP_POW);
        tree_leaf_t* num_leaf = new_leaf (TYPE_NUM, (diff_leaf->right->value).data);
        tree_leaf_t* degree_leaf = new_leaf (TYPE_NUM, (diff_leaf->right->value).data - 1);

        add_leaf (diff_tree, pow_leaf, copy_leaf (diff_tree, diff_leaf->left), "l");
        add_leaf (diff_tree, pow_leaf, degree_leaf, "r");

        add_leaf (diff_tree, mul_leaf, pow_leaf, "r");
        add_leaf (diff_tree, mul_leaf, num_leaf, "l");

        tree_leaf_t* ext_mul = new_leaf (TYPE_OPER, OP_MUL);
        add_leaf (diff_tree, ext_mul, mul_leaf, "l");
        tree_leaf_t* l = diff (diff_tree, diff_leaf->left);
        add_leaf (diff_tree, ext_mul, l, "r");

        return ext_mul;
    }

    if ((diff_leaf->value).type == TYPE_FUNC)
    {
        tree_leaf_t* func_leaf = NULL;
        tree_leaf_t* mul_leaf = new_leaf (TYPE_OPER, OP_MUL);

        add_leaf (diff_tree, mul_leaf, diff (diff_tree, diff_leaf->left), "r");

        switch ((int)(diff_leaf->value).data)
        {
            case FUNC_SIN:
                func_leaf = new_leaf (TYPE_FUNC, FUNC_COS);

                add_leaf (diff_tree, func_leaf, copy_leaf (diff_tree, diff_leaf->left), "l");
                add_leaf (diff_tree, mul_leaf, func_leaf, "l");

                break;

            case FUNC_COS:
            {
                tree_leaf_t* neg_leaf = new_leaf (TYPE_NUM, -1);
                tree_leaf_t* ext_mul = new_leaf (TYPE_OPER, OP_MUL);
                func_leaf = new_leaf (TYPE_FUNC, FUNC_SIN);

                add_leaf (diff_tree, ext_mul, neg_leaf, "l");
                add_leaf (diff_tree, func_leaf, copy_leaf (diff_tree, diff_leaf->left), "l");
                add_leaf (diff_tree, ext_mul, func_leaf, "r");
                add_leaf (diff_tree, mul_leaf, ext_mul, "l");

                break;
            }

            case FUNC_TG:
            {
                tree_leaf_t* div_leaf = new_leaf (TYPE_OPER, OP_DIV);
                tree_leaf_t* num_leaf = new_leaf (TYPE_NUM, 1);
                tree_leaf_t* pow_leaf = new_leaf (TYPE_OPER, OP_POW);
                tree_leaf_t* cos_leaf = new_leaf (TYPE_FUNC, FUNC_COS);
                tree_leaf_t* degree_leaf = new_leaf (TYPE_NUM, 2);

                add_leaf (diff_tree, cos_leaf, copy_leaf (diff_tree, diff_leaf->left), "l");
                add_leaf (diff_tree, pow_leaf, cos_leaf, "l");
                add_leaf (diff_tree, pow_leaf, degree_leaf, "r");

                add_leaf (diff_tree, div_leaf, pow_leaf, "r");
                add_leaf (diff_tree, div_leaf, num_leaf, "l");

                add_leaf (diff_tree, mul_leaf, div_leaf, "l");

                break;
            }

            case FUNC_LN:
            {
                tree_leaf_t* div_leaf = new_leaf (TYPE_OPER, OP_DIV);
                tree_leaf_t* num_leaf = new_leaf (TYPE_NUM, 1);

                add_leaf (diff_tree, div_leaf, num_leaf, "l");

                add_leaf (diff_tree, div_leaf, copy_leaf (diff_tree, diff_leaf->left), "r");

                add_leaf (diff_tree, mul_leaf, div_leaf, "l");

                break;
            }

            case FUNC_SQRT:
            {
                tree_leaf_t* div_leaf = new_leaf (TYPE_OPER, OP_DIV);
                tree_leaf_t* sqrt_leaf = new_leaf (TYPE_FUNC, FUNC_SQRT);
                tree_leaf_t* ext_mul = new_leaf (TYPE_OPER, OP_MUL);
                tree_leaf_t* num_leaf = new_leaf (TYPE_NUM, 1);
                tree_leaf_t* two_leaf = new_leaf (TYPE_NUM, 2);

                add_leaf (diff_tree, ext_mul, two_leaf, "l");
                add_leaf (diff_tree, sqrt_leaf, copy_leaf (diff_tree, diff_leaf->left), "l");
                add_leaf (diff_tree, ext_mul, sqrt_leaf, "r");
                add_leaf (diff_tree, div_leaf, num_leaf, "l");
                add_leaf (diff_tree, div_leaf, ext_mul, "r");

                add_leaf (diff_tree, mul_leaf, div_leaf, "l");
            }
        }

        return mul_leaf;
    }
}

void tree_opt (tree_t* source_tree, tree_leaf_t* leaf)
{
    while (1)
    {
        tree_leaf_t* const_leaf = tree_const (source_tree, leaf);
        if (const_leaf)
        {
            tree_leaf_t* par_leaf = leaf->parent;

            if (par_leaf->left == leaf)
            {
                delete_leaf (source_tree, leaf);
                add_leaf (source_tree, par_leaf, const_leaf, "l");
            }

            if (par_leaf->right == leaf)
            {
                delete_leaf (source_tree, leaf);
                add_leaf (source_tree, par_leaf, const_leaf, "r");
            }
        }

        tree_leaf_t* oper_leaf = del_oper (source_tree, leaf);
        if (oper_leaf)
        {
            tree_leaf_t* par_leaf = leaf->parent;

            if (par_leaf->left == leaf)
            {
                delete_leaf (source_tree, leaf);
                add_leaf (source_tree, par_leaf, oper_leaf, "l");
            }

            if (par_leaf->right == leaf)
            {
                delete_leaf (source_tree, leaf);
                add_leaf (source_tree, par_leaf, oper_leaf, "r");
            }
        }

        if (CHNG_FLAG)
            CHNG_FLAG = 0;
        else
            break;
    }

    return;
}

tree_leaf_t* tree_const (tree_t* source_tree, tree_leaf_t* leaf)
{
    if (((leaf->value).type == TYPE_VAR) || ((leaf->value).type == TYPE_NUM))
        return NULL;

    if (((leaf->value).type == TYPE_OPER) && (((leaf->left->value).type == TYPE_NUM) && ((leaf->right->value).type == TYPE_NUM)))
    {
        CHNG_FLAG = 1;
        tree_leaf_t* const_leaf = NULL;

        switch ((int) (leaf->value).data)
        {
            case OP_PLUS:
                const_leaf = new_leaf (TYPE_NUM, (leaf->left->value).data + (leaf->right->value).data);
                break;

            case OP_MINUS:
                const_leaf = new_leaf (TYPE_NUM, (leaf->left->value).data - (leaf->right->value).data);
                break;

            case OP_MUL:
                const_leaf = new_leaf (TYPE_NUM, (leaf->left->value).data * (leaf->right->value).data);
                break;

            case OP_DIV:
                const_leaf = new_leaf (TYPE_NUM, (leaf->left->value).data / (leaf->right->value).data);
                break;

            case OP_POW:
                const_leaf = new_leaf (TYPE_NUM, pow ((leaf->left->value).data, (leaf->right->value).data));
                break;
        }
        return const_leaf;
    }

    if (((leaf->value).type == TYPE_FUNC) && ((leaf->left->value).type == TYPE_NUM))
    {
        CHNG_FLAG = 1;
        tree_leaf_t* const_leaf = NULL;

        switch ((int) (leaf->value).data)
        {
            case FUNC_SIN:
                const_leaf = new_leaf (TYPE_NUM, sin ((leaf->left->value).data));
                break;

            case FUNC_COS:
                const_leaf = new_leaf (TYPE_NUM, cos ((leaf->left->value).data));
                break;

            case FUNC_TG:
                const_leaf = new_leaf (TYPE_NUM, tan ((leaf->left->value).data));
                break;

            case FUNC_LN:
                const_leaf = new_leaf (TYPE_NUM, log ((leaf->left->value).data));
                break;

            case FUNC_SQRT:
                const_leaf = new_leaf (TYPE_NUM, sqrt ((leaf->left->value).data));
                break;
        }
        return const_leaf;
    }

    if ((leaf->value).type == TYPE_OPER)
    {
        tree_leaf_t* left_const = tree_const (source_tree, leaf->left);
        if (left_const)
        {
            delete_leaf (source_tree, leaf->left);
            add_leaf (source_tree, leaf, left_const, "l");
        }

        tree_leaf_t* right_const = tree_const (source_tree, leaf->right);
        if (right_const)
        {
            delete_leaf (source_tree, leaf->right);
            add_leaf (source_tree, leaf, right_const, "r");
        }

        if (((leaf->left->value).type == TYPE_NUM) && ((leaf->right->value).type == TYPE_NUM))
        {
            tree_leaf_t* res_leaf = tree_const (source_tree, leaf);
            return res_leaf;
        }
        else
            return NULL;
    }

    if ((leaf->value).type == TYPE_FUNC)
    {
        tree_leaf_t* left_const = tree_const (source_tree, leaf->left);

        if (left_const)
        {
            delete_leaf (source_tree, leaf->left);
            add_leaf (source_tree, leaf, left_const, "l");
        }

        if ((leaf->left->value).type == TYPE_NUM)
        {
            tree_leaf_t* res_leaf = tree_const (source_tree, leaf);
            return res_leaf;
        }
        else
            return NULL;
    }

    return NULL;
}

tree_leaf_t* del_oper (tree_t* source_tree, tree_leaf_t* leaf)
{
    if (((leaf->value).type != TYPE_OPER))
        return NULL;

    if (leaf && (leaf->value).type == TYPE_OPER &&
     (((leaf->left->value).type != TYPE_NUM || (leaf->right->value).type != TYPE_NUM)))
    {
        tree_leaf_t* left_opt = NULL;
        if (leaf->left && ((leaf->left->value).type == TYPE_OPER))
            left_opt = del_oper (source_tree, leaf->left);

        tree_leaf_t* right_opt = NULL;
        if (leaf->right && ((leaf->right->value).type == TYPE_OPER))
            right_opt = del_oper (source_tree, leaf->right);

        if (left_opt)
        {
            delete_leaf (source_tree, leaf->left);
            add_leaf (source_tree, leaf, left_opt, "l");
        }

        if (right_opt)
        {
            delete_leaf (source_tree, leaf->right);
            add_leaf (source_tree, leaf, right_opt, "r");
        }


        if ((((leaf->value).data == OP_MUL) &&
        (((((leaf->left->value).type == TYPE_NUM) && (((leaf->left->value).data == 0) || (leaf->left->value).data == 1)) ||
        (((((leaf->right->value).type == TYPE_NUM) && (((leaf->right->value).data == 0) || (leaf->right->value).data == 1))))))) ||

        (((leaf->value).data == OP_PLUS) &&
        ((((leaf->left->value).type == TYPE_NUM) && (leaf->left->value).data == 0) ||
        ((((leaf->right->value).type == TYPE_NUM) && (leaf->right->value).data == 0)))) ||

        (((leaf->value).data == OP_MINUS) &&
        ((((leaf->right->value).type == TYPE_NUM) && (leaf->right->value).data == 0))) ||

        (((leaf->value).data == OP_DIV) &&
        ((((leaf->left->value).type == TYPE_NUM) && (leaf->left->value).data == 0) ||
        ((((leaf->right->value).type == TYPE_NUM) && (leaf->right->value).data == 1)))) ||

        (((leaf->value).data == OP_POW) &&
        ((((leaf->right->value).type == TYPE_NUM) && (leaf->right->value).data == 1))))

        {
            if (((leaf->value).type == TYPE_OPER) &&
            (((leaf->left->value).type == TYPE_NUM) || (((leaf->right->value).type == TYPE_NUM))))
            {
                if ((((leaf->value).type == TYPE_OPER) && (((leaf->value).data == OP_MUL))) &&
                ((((leaf->left->value).type == TYPE_NUM) && ((leaf->left->value).data == 0)) ||
                (((leaf->right->value).type == TYPE_NUM) && ((leaf->right->value).data == 0))))
                {
                    CHNG_FLAG = 1;
                    tree_leaf_t* opt_leaf = new_leaf (TYPE_NUM, 0);
                    return opt_leaf;
                }

                if ((((leaf->value).type == TYPE_OPER) && (((leaf->value).data == OP_MUL))) &&
                ((((leaf->left->value).type == TYPE_NUM) && ((leaf->left->value).data == 1)) ||
                (((leaf->right->value).type == TYPE_NUM) && ((leaf->right->value).data == 1))))
                {
                    CHNG_FLAG = 1;

                    if ((leaf->left->value).data == 1)
                    {
                        tree_leaf_t* opt_leaf = copy_leaf (source_tree, leaf->right);
                        return opt_leaf;
                    }

                    if ((leaf->right->value).data == 1)
                    {
                        tree_leaf_t* opt_leaf = copy_leaf (source_tree, leaf->left);
                        return opt_leaf;
                    }
                }

                if ((((leaf->value).type == TYPE_OPER) && ((leaf->value).data == OP_DIV)) &&
                    ((leaf->left->value).type == TYPE_NUM && (leaf->left->value).data == 0))
                {
                    CHNG_FLAG = 1;
                    tree_leaf_t* opt_leaf = new_leaf (TYPE_NUM, 0);
                    return opt_leaf;
                }

                if ((((leaf->value).type == TYPE_OPER) && ((leaf->value).data == OP_DIV || (leaf->value).data == OP_POW)) &&
                    (((leaf->right->value).type == TYPE_NUM && (leaf->right->value).data == 1)))
                {
                    CHNG_FLAG = 1;
                    tree_leaf_t* opt_leaf = copy_leaf (source_tree, leaf->left);
                    return opt_leaf;
                }

                if ((((leaf->value).type == TYPE_OPER) && ((leaf->value).data == OP_PLUS)) &&
                    ((((leaf->left->value).type == TYPE_NUM) && (leaf->left->value).data == 0) ||
                    (((leaf->right->value).type == TYPE_NUM) && (leaf->right->value).data == 0)))
                {
                    CHNG_FLAG = 1;

                    if ((leaf->left->value).data == 0)
                    {
                        tree_leaf_t* opt_leaf = copy_leaf (source_tree, leaf->right);
                        return opt_leaf;
                    }

                    if ((leaf->right->value).data == 0)
                    {
                        tree_leaf_t* opt_leaf = copy_leaf (source_tree, leaf->left);
                        return opt_leaf;
                    }
                }

                if ((((leaf->value).type == TYPE_OPER) && ((leaf->value).data == OP_MINUS)) &&
                    (((leaf->right->value).type == TYPE_NUM) && (leaf->right->value).data == 0))
                {
                    CHNG_FLAG = 1;
                    tree_leaf_t* opt_leaf = copy_leaf (source_tree, leaf->left);
                    return opt_leaf;
                }
            }
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

void lexycal_analysis ()
{
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

    return;
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

void unknown_comand (char inv_str [])
{
    printf ("#UNKNOWN COMMAND: %s\n", inv_str);
    abort();
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

void leaf_out (tree_leaf_t* leaf)
{
    int cnt = 0;
    #include "chk_func.h"
    #include "chk_ops.h"

    switch ((leaf->value).type)
    {
        case TYPE_NUM:
            if ((leaf->value).data < 0)
            {
                fprintf (OUTPUT, "(");
                fprintf (OUTPUT, "%g", (leaf->value).data);
                fprintf (OUTPUT, ")");
            }
            else
                fprintf (OUTPUT, "%g", (leaf->value).data);

            break;

        case TYPE_VAR:
            fputc ((int)(leaf->value).data + 'a', OUTPUT);
            break;

        case TYPE_OPER:
        {
            int prior_flag = 0, pow_flag = 0;

            switch ((int) (leaf->value).data)
            {
                case OP_PLUS:
                case OP_MINUS:
                    leaf_out (leaf->left);
                    if ((leaf->value).data == OP_PLUS)
                        fprintf (OUTPUT, " + ");
                    else
                        fprintf (OUTPUT, " - ");
                    leaf_out (leaf->right);

                    break;

                case OP_MUL:
                    if (((leaf->left->value).type == TYPE_NUM) || (leaf->right->value).type == TYPE_NUM)
                    {
                        if (((leaf->left->value).type == TYPE_NUM) && (!any_num (leaf->right)))
                        {
                            leaf_out (leaf->left);
                            leaf_out (leaf->right);

                            break;
                        }

                        if (((leaf->right->value).type == TYPE_NUM) && (!any_num (leaf->left)))
                        {
                            leaf_out (leaf->right);
                            leaf_out (leaf->left);

                            break;
                        }
                    }

                    if ((leaf->left->value).type == TYPE_OPER)
                        if (get_prior (leaf->left) < get_prior (leaf))
                        {
                            prior_flag = 1;
                            fprintf (OUTPUT, "(");
                        }
                    leaf_out (leaf->left);
                    if (prior_flag)
                    {
                        fprintf (OUTPUT, ")");
                        prior_flag = 0;
                    }

                    fprintf (OUTPUT, " \\cdot ");

                    if ((leaf->right->value).type == TYPE_OPER)
                        if (get_prior (leaf->right) < get_prior (leaf))
                        {
                            prior_flag = 1;
                            fprintf (OUTPUT, "(");
                        }
                    leaf_out (leaf->right);
                    if (prior_flag)
                    {
                        fprintf (OUTPUT, ")");
                        prior_flag = 0;
                    }

                    break;

                case OP_DIV:
                    fprintf (OUTPUT, " \\displaystyle {\\frac {");
                    leaf_out (leaf->left);
                    fprintf (OUTPUT, "}{");
                    leaf_out (leaf->right);
                    fprintf (OUTPUT, "}}");
                    break;

                case OP_POW:
                    if ((leaf->left->value).type == TYPE_FUNC)
                    {
                        for (cnt = 0; cnt <= NUM_FUNC - 1; cnt++)
                            if ((leaf->left->value).data == (functions [cnt]).num)
                            {
                                fprintf (OUTPUT, "\\%s", (functions [cnt]).str);
                                break;
                            }
                        fprintf (OUTPUT, "^{");
                        leaf_out (leaf->right);
                        fprintf (OUTPUT, "}(");
                        leaf_out (leaf->left->left);
                        fprintf (OUTPUT, ")");

                        break;
                    }

                    if ((leaf->left->value).type == TYPE_OPER && (leaf->left->value).data == OP_POW)
                    {
                        (leaf->left->right->value).data *= (leaf->right->value).data;
                        (leaf->right->value).data = 1;
                        leaf_out (leaf->left);

                        break;
                    }

                    if (((leaf->left->value).type) != TYPE_NUM && (leaf->left->value).type != TYPE_VAR)
                    {
                        pow_flag = 1;
                        fprintf (OUTPUT, "(");
                    }
                    leaf_out (leaf->left);
                    if (pow_flag)
                    {
                        fprintf (OUTPUT, ")");
                        pow_flag = 0;
                    }

                    fprintf (OUTPUT, "^{");
                    leaf_out (leaf->right);
                    fprintf (OUTPUT, "}");

                    break;
            }
            break;
        }

        case TYPE_FUNC:
            for (cnt = 0; cnt <= NUM_FUNC - 1; cnt++)
                if ((leaf->value).data == (functions [cnt]).num)
                {
                    fprintf (OUTPUT, "\\%s", (functions [cnt]).str);
                    break;
                }
            fprintf (OUTPUT, "{(");
            leaf_out (leaf->left);
            fprintf (OUTPUT, "})");
            break;

        default:
            return;
    }

    return;
}

int get_prior (tree_leaf_t* leaf)
{
    if ((leaf->value).type != TYPE_OPER)
        return 0;

    switch ((int) (leaf->value).data)
    {
        case OP_PLUS:
        case OP_MINUS:
            return 1;

        case OP_MUL:
        case OP_DIV:
            return 2;

        case OP_POW:
            return 3;

        default:
            return 0;
    }
}

void Dmitry_Vladimirovich ()
{
    char buf = "";

    tree_t* source_tree = new_tree ();
    add_leaf (source_tree, source_tree->root, Get_E (source_tree), "l");

    while (buf = fgetc (PATTERN))
    {
        if (buf == '#')
        {
            leaf_out (source_tree->root->left);
            break;
        }
        else
            fputc (buf, OUTPUT);
    }

    tree_t* diff_tree = new_tree ();
    add_leaf (diff_tree, diff_tree->root, diff (diff_tree, source_tree->root->left), "l");

    while (buf = fgetc (PATTERN))
    {
        if (buf == '@')
        {
            leaf_out (diff_tree->root->left);
            break;
        }
        else
            fputc (buf, OUTPUT);
    }

    tree_opt (diff_tree, diff_tree->root->left);

    while ((buf = fgetc (PATTERN)) != EOF)
    {
        if (buf == '%')
            leaf_out (diff_tree->root->left);
        else
            fputc (buf, OUTPUT);
    }

    return;
}

int any_num (tree_leaf_t* leaf)
{
    if ((leaf->value).type == TYPE_NUM ||
        (leaf->left && ((leaf->left->value).type == TYPE_NUM)) ||
        (leaf->right && (leaf->right->value).type == TYPE_NUM))
            return 1;
    else
        if (!((((leaf->value).type == TYPE_OPER) &&
        ((leaf->value).data == OP_MUL || (leaf->value).data == OP_PLUS || (leaf->value).data == OP_MINUS))))
            return 0;
        else
            return (any_num (leaf->left) || any_num (leaf->right));
}
