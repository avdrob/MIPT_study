#include "op_num.h"

typedef struct
{
    char* str;
    int num;
} oper_id;

oper_id operators [] = {{"<", OP_LESS}, {">", OP_MORE}, {"<=", OP_LES_EQ}, {">=", OP_MOR_EQ}, {"+", OP_PLUS},
                      {"-", OP_MINUS}, {"*", OP_MUL}, {"=", OP_EQ}, {"><", OP_NEQ}, {";", OP_SEP},
                      {"/", OP_DIV},  {"(", OP_OPEN_RND_BR}, {")", OP_CLOSE_RND_BR},
                      {"{", OP_OPEN_FIG_BR}, {"}", OP_CLOSE_FIG_BR}, {"^", OP_POW},
                      {"if", OP_IF}, {"while", OP_WHILE}, {"ret", OP_RET}};

#define CHK_OPER(oper)                                                      \
        {                                                                   \
            int flag = 0;                                                   \
            for (cnt2 = 0; cnt2 <= NUM_OPER - 1; cnt2++)                    \
                if (!strcmp ((operators [cnt2]).str, oper))                 \
                {                                                           \
                    (CODE [TOKEN_CNT]).data = (operators [cnt2]).num;       \
                    (CODE [TOKEN_CNT++]).type = TYPE_OPER;                  \
                    flag = 1;                                               \
                    break;                                                  \
                }                                                           \
                                                                            \
            if (flag)                                                       \
                continue;                                                   \
        }

int NUM_OPER = sizeof (operators) / sizeof (oper_id), cnt2 = 0;
